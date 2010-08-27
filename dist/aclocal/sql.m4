
# Process sql API specific settings.
AC_DEFUN(AC_SQL_CONFIG, [
# Setup the SQLite debug build.
mkdir -p sql
if test "$with_tclconfig" != "no"; then
	db_cv_sql_config_tclconfig="--with-tcl=$with_tclconfig"
else
	db_cv_sql_config_tclconfig=	
fi

# It would be nice to use AC_CONFIG_SUBDIRS here, but it does not allow for
# tweaking of command line options, so hard code things instead.
#
# !!! BEGIN COPIED from autoconf distribution
# Modified to not repeat CPPFLAGS or readline settings

  # Remove --cache-file, --srcdir, and --disable-option-checking arguments
  # so they do not pile up.
  ac_sub_configure_args=
  ac_prev=
  eval "set x $ac_configure_args"
  shift
  for ac_arg
  do
    if test -n "$ac_prev"; then
      ac_prev=
      continue
    fi
    case $ac_arg in
    -cache-file | --cache-file | --cache-fil | --cache-fi \
    | --cache-f | --cache- | --cache | --cach | --cac | --ca | --c)
      ac_prev=cache_file ;;
    -cache-file=* | --cache-file=* | --cache-fil=* | --cache-fi=* \
    | --cache-f=* | --cache-=* | --cache=* | --cach=* | --cac=* | --ca=* \
    | --c=*)
      ;;
    --config-cache | -C)
      ;;
    -srcdir | --srcdir | --srcdi | --srcd | --src | --sr)
      ac_prev=srcdir ;;
    -srcdir=* | --srcdir=* | --srcdi=* | --srcd=* | --src=* | --sr=*)
      ;;
    -prefix | --prefix | --prefi | --pref | --pre | --pr | --p)
      ac_prev=prefix ;;
    -prefix=* | --prefix=* | --prefi=* | --pref=* | --pre=* | --pr=* | --p=*)
      ;;
    --disable-option-checking)
      ;;
    CPPFLAGS=* | *readline*)
      ;;
    *)
      case $ac_arg in
      *\'*) ac_arg=`AS_ECHO(["$ac_arg"]) | sed "s/'/'\\\\\\\\''/g"` ;;
      esac
      ac_sub_configure_args="$ac_sub_configure_args '$ac_arg'" ;;
    esac
  done

  # Always prepend --prefix to ensure using the same prefix
  # in subdir configurations.
  ac_arg="--prefix=$prefix"
  case $ac_arg in
  *\'*) ac_arg=`AS_ECHO(["$ac_arg"]) | sed "s/'/'\\\\\\\\''/g"` ;;
  esac
  ac_sub_configure_args="'$ac_arg' $ac_sub_configure_args"

  # Pass --silent
  if test "$silent" = yes; then
    ac_sub_configure_args="--silent $ac_sub_configure_args"
  fi

  # Always prepend --disable-option-checking to silence warnings, since
  # different subdirs can have different --enable and --with options.
  ac_sub_configure_args="--disable-option-checking $ac_sub_configure_args"

# !!! END COPIED from autoconf distribution

sqlite_dir=`cd $srcdir/../lang/sql/sqlite && /bin/pwd`
(cd sql && eval "\$SHELL $sqlite_dir/configure --disable-option-checking $ac_sub_configure_args CPPFLAGS=\"-I.. $CPPFLAGS\" --enable-amalgamation=$db_cv_sql_amalgamation --enable-readline=$with_readline" && cat build_config.h >> config.h)

# Configure JDBC if --enable-jdbc
if test "$db_cv_jdbc" != "no"; then

  # Deal with user-defined jdbc source path
  if test "$with_jdbc" != "no"; then
    jdbc_path="$with_jdbc"
  else
    jdbc_path="$srcdir/../lang/sql/jdbc"
  fi

  if test ! -d $jdbc_path; then
    echo "Cannot find jdbc source in $jdbc_path; please check that path or use --with-jdbc to specify the source directory"
    exit 1
  fi
  jdbc_dir=`cd $jdbc_path && /bin/pwd`

  # Transfer following setting to jdbc configure:
  # . --prefix
  # . --enable-shared/--disable-shared
  # . --enable-static/--disable-static
  # . CFLAGS, CPPFLAGS and LDFLAGS
  jdbc_args=""
  jdbc_flags=""

  test "$prefix" != "" && jdbc_args="--prefix=$prefix --with-jardir=$prefix/jar"
  test "$enable_shared" != "" && jdbc_args="$jdbc_args --enable-shared=$enable_shared"
  test "$enable_static" != "" && jdbc_args="$jdbc_args --enable-static=$enable_static"

  jdbc_flags="$jdbc_flags CFLAGS=\"-I.. -I../src/dbinc -DHAVE_SQLITE3_MALLOC -DHAVE_ERRNO_H $CFLAGS\""
  jdbc_flags="$jdbc_flags CPPFLAGS=\"-I.. $CPPFLAGS\""
  test "$LDFLAGS" != "" && jdbc_flags="$jdbc_flags LDFLAGS=\"$LDFLAGS\""

  # Copy ../lang/sql/jdbc to build_unix/
  test ! -d jdbc && cp -r $jdbc_dir .

  # Set DBSQL LIB for Makefile.in
  BDB_LIB="..\/libdb-$DB_VERSION_MAJOR.$DB_VERSION_MINOR.la"
  test $enable_shared != "yes" && BDB_LIB='..\/libdb.a'

  # Run the jdbc/configure
  cd jdbc
  test ! -e Makefile.in.tmp && mv Makefile.in Makefile.in.tmp
  sed "s/@BDB_LIB@/$BDB_LIB/g" Makefile.in.tmp > Makefile.in
  eval "\$SHELL ./configure --with-sqlite3=../../lang/sql/generated $jdbc_args $jdbc_flags"
fi
])
