find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GLIB2 glib-2.0)
endif (PKG_CONFIG_FOUND)
