#! /bin/sh
###########################################################################
# Copyright (C) 2001-2003 Peter J Jones (pjones@pmade.org)
# All Rights Reserved
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name of the Author nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
###########################################################################

PROJECT=xmlwrapp
VERSION_FILE=docs/VERSION
VERSION=`head -1 $VERSION_FILE | perl -pe 's/\s+.*$//'`
CVSTAG=`head -1 $VERSION_FILE  | perl -pe 's/^[\d.]+\s+\d+\s+\d+\s+([^\s]+).*$/$1/'`
DESTDIR="$PROJECT-$VERSION"
DEFAULT_FDP="${HOME}/develop/freebsd-doc-project"
DOCBOOK_XSL=$HOME/develop/xslt-tools/stylesheets/docbook2html.xsl
README_XSL=$HOME/develop/project-xslt/stylesheets/readme.xsl
W3M="w3m -cols 76 -dump"
XSLT="xsltproc"

echo "===> Making $PROJECT release (Version == $VERSION) (CVSTAG == $CVSTAG)"

if [ ! -r $DOCBOOK_XSL ] ; then
    echo "missing $DOCBOOK_XSL"
    exit 1
fi

if [ ! -r $README_XSL ] ; then
    echo "missing $README_XSL"
    exit 1
fi

# checkout the correct tag/branch from CVS
cvs -q checkout -r $CVSTAG -d $DESTDIR $PROJECT

# remove the CVS directories
find $DESTDIR -name CVS -type d | xargs rm -rf
find $DESTDIR -name '.cvsignore' -type f | xargs rm -f

# create the README, INSTALL and like files
TMPFILE=tmp.html
(
    cd $DESTDIR

    $XSLT $README_XSL docs/project/project.xml > $TMPFILE
    $W3M $TMPFILE > README

    $XSLT $DOCBOOK_XSL docs/manual/quickstart.xml > $TMPFILE
    $W3M $TMPFILE > INSTALL

    $XSLT $DOCBOOK_XSL docs/manual/credits.xml > $TMPFILE
    $W3M $TMPFILE > docs/CREDITS

    $XSLT $DOCBOOK_XSL docs/manual/todo.xml > $TMPFILE
    $W3M $TMPFILE > docs/TODO

    rm $TMPFILE
)

# create a tarball
tar czvf ${DESTDIR}.tar.gz $DESTDIR

# remove the directory that CVS created
rm -rf $DESTDIR
