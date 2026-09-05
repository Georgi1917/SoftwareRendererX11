meson_setup_builddir() {
    meson setup builddir
}

meson_configure() {
    pushd builddir 1>/dev/null
    meson configure $@
    popd 1>/dev/null
}

meson_cleanup() {
    rm -rf builddir
}

meson_build() {
    pushd builddir 1>/dev/null
    meson compile
    popd 1>/dev/null
}