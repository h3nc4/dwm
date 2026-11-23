#!/bin/sh

# Builds dwm-h3nc4_<version>_<arch>.deb from what make install puts under /usr. CI runs it on
# trixie, the oldest suite served, since dpkg-shlibdeps asks for the build host's library versions.

set -eu

cd "$(dirname "$0")/../"

package=dwm-h3nc4
upstream="$(sed -n 's/^VERSION[[:space:]]*=[[:space:]]*//p' config.mk)"
when="$(TZ=UTC git log -1 --format=%cd --date=format-local:%Y%m%d%H%M%S)"
commit="$(git rev-parse HEAD)"
short="$(printf '%s' "${commit}" | cut -c1-8)"
# Builds sort by the time of their commit, and the hash matches the release tag.
version="${upstream}+git${when}.${short}"
arch="$(dpkg --print-architecture)"
SOURCE_DATE_EPOCH="$(git log -1 --format=%ct)"
DEB_BUILD_MAINT_OPTIONS=hardening=+all
export SOURCE_DATE_EPOCH DEB_BUILD_MAINT_OPTIONS

work="$(mktemp -d)"
trap 'rm -rf "${work}"' EXIT
root="${work}/root"
doc="${root}/usr/share/doc/${package}"

cppflags="$(dpkg-buildflags --get CPPFLAGS)"
cflags="$(dpkg-buildflags --get CFLAGS)"
ldflags="$(dpkg-buildflags --get LDFLAGS)"
stamp="$(date -u -R -d "@${SOURCE_DATE_EPOCH}")"

make clean
make CC="cc ${cppflags} ${cflags} ${ldflags}" \
  PREFIX=/usr MANPREFIX=/usr/share/man DESTDIR="${root}" install
strip --remove-section=.comment --remove-section=.note --strip-unneeded "${root}/usr/bin/dwm"
gzip -9n "${root}/usr/share/man/man1/dwm.1"

install -Dm644 LICENSE "${doc}/copyright"
printf '%s (%s) unstable; urgency=medium\n\n  * Build of %s.\n\n -- %s  %s\n' \
  "${package}" "${version}" "${commit}" "Dreamweaver <me@h3nc4.com>" "${stamp}" |
  gzip -9n >"${doc}/changelog.gz"
chmod 644 "${doc}/changelog.gz"

mkdir -p "${work}/debian" "${root}/DEBIAN"
printf 'Source: %s\n\nPackage: %s\nArchitecture: %s\n' "${package}" "${package}" "${arch}" \
  >"${work}/debian/control"
depends="$(cd "${work}" && dpkg-shlibdeps -O root/usr/bin/dwm | sed -n 's/^shlibs:Depends=//p')"
size="$(du -sk --exclude=DEBIAN "${root}" | cut -f1)"

cat >"${root}/DEBIAN/control" <<EOF
Package: ${package}
Version: ${version}
Architecture: ${arch}
Maintainer: Dreamweaver <me@h3nc4.com>
Installed-Size: ${size}
Depends: ${depends}
Provides: x-window-manager
Conflicts: dwm
Replaces: dwm
Section: x11
Priority: optional
Homepage: https://github.com/h3nc4/dwm
Description: tiling window manager for X, h3nc4 build
 dwm arranges windows in tiled, monocle and floating layouts across nine tags.
 This build adds the pertag, uselessgap, movestack, alpha and other patches
 and the config.h that Dreamweaver installs.
EOF

cat >"${root}/DEBIAN/postinst" <<'EOF'
#!/bin/sh
set -e
if [ "$1" = configure ]; then
  update-alternatives --install /usr/bin/x-window-manager x-window-manager /usr/bin/dwm 20 \
    --slave /usr/share/man/man1/x-window-manager.1.gz x-window-manager.1.gz /usr/share/man/man1/dwm.1.gz
fi
EOF

cat >"${root}/DEBIAN/prerm" <<'EOF'
#!/bin/sh
set -e
if [ "$1" = remove ] || [ "$1" = deconfigure ]; then
  update-alternatives --remove x-window-manager /usr/bin/dwm
fi
EOF
chmod 755 "${root}/DEBIAN/postinst" "${root}/DEBIAN/prerm"

find "${root}" -type d -exec chmod 755 {} +
(cd "${root}" && find usr -type f -exec md5sum {} + | sort -k2) >"${root}/DEBIAN/md5sums"
chmod 644 "${root}/DEBIAN/md5sums"
dpkg-deb --root-owner-group --build "${root}" "${package}_${version}_${arch}.deb"
make clean
