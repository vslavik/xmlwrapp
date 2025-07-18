# Used to install dependencies for the CI builds under Debian/Ubuntu.

run_apt_install() {
    echo "-> Running apt-get install $@"

    # Disable some (but not all) output.
    sudo apt-get -q -o=Dpkg::Use-Pty=0 install -y "$@"

    rc=$?
    echo "-> Done with $rc"

    return $rc
}

# Arch-independent dependencies.
sudo apt-get update -qq

if [ "$XMLWRAPP_WITH_DOCS" = 1 ]; then
    doc_packages='doxygen graphviz'
fi

case "$HOST" in
    *-w64-mingw32)
        arch=`echo $HOST | sed -e 's/-w64-mingw32$//;s/_/-/'`

        case "$arch" in
            i686)
                sudo dpkg --add-architecture i386

                # Workaround for https://github.com/actions/runner-images/issues/4589
                # (normally only apt-get update should have been needed)
                sudo rm -f /etc/apt/sources.list.d/microsoft-prod.list
                sudo apt-get update -qq
                sudo apt-get remove -qq libmono* moby* mono* nginx* php* libgdiplus libpcre2-posix3 libzip4

                wine_package=wine32-development
                ;;

            x86-64)
                wine_package=wine64-development
                ;;

            *)
                echo "Unknown architecture $arch when cross-compiling"
                exit 1
                ;;
        esac

        run_apt_install $doc_packages g++-mingw-w64-$arch $wine_package wine-development

        echo -n "Cross-compiling for $HOST using "
        $HOST-g++ --version

        # Download the previously built required dependencies.
        wget --no-verbose https://github.com/vslavik/xmlwrapp/releases/download/xmllibs/xmllibs-${HOST}.zip
        sudo unzip -q xmllibs-${HOST}.zip -d /usr/local
        ;;

    *)
        # Assume native build.
        run_apt_install $doc_packages libxml2-dev libxslt1-dev
        ;;
esac
