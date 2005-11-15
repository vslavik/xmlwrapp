<html><head>
<!-- ViewCVS -- http://viewcvs.sourceforge.net/
by Greg Stein -- mailto:gstein@lyra.org
-->

<title>project-xslt/etc/project.mk - view - 1.2</title>
</head>
<body bgcolor="#eeeeee">
<table width="100%" border=0 cellspacing=0 cellpadding=1 bgcolor="#9999ee">
<tr valign=bottom>
<td>
<a href="project.mk#rev1.2">
<img src="/icons/small/back.gif" alt="(file)" border=0 width=16 height=16>
</a>
<b>Return to
<a href="project.mk#rev1.2">project.mk</a>
CVS log</b>
<img src="/icons/small/text.gif" alt="(file)" border=0 width=16 height=16>
</td>
<td align=right>
<img src="/icons/small/dir.gif" alt="(dir)" border=0 width=16 height=16>
<b>Up to <a href="/cgi-cvs/viewcvs.cgi/#dirlist">[Main]</a> / <a href="/cgi-cvs/viewcvs.cgi/project-xslt/#dirlist">project-xslt</a> / <a href="/cgi-cvs/viewcvs.cgi/project-xslt/etc/#dirlist">etc</a></b>
</td>
</tr>
</table>

<hr noshade>
<table width="100%"><tr><td bgcolor="#ffffff">
File: <a href="/cgi-cvs/viewcvs.cgi/#dirlist">[Main]</a> / <a href="/cgi-cvs/viewcvs.cgi/project-xslt/#dirlist">project-xslt</a> / <a href="/cgi-cvs/viewcvs.cgi/project-xslt/etc/#dirlist">etc</a> / <a href="/cgi-cvs/viewcvs.cgi/project-xslt/etc/project.mk#dirlist">project.mk</a>
(<a href="/cgi-cvs/viewcvs.cgi/*checkout*/project-xslt/etc/project.mk?rev=1.2" target="cvs_checkout"
onClick="window.open('about:blank','cvs_checkout',
'resizeable=1,scrollbars=1');"
><b>download</b></a>)


<br>

Revision: <b>1.2</b>,
<i>Mon Aug 25 19:26:36 2003 UTC</i> (2 years ago) by <i>pjones</i>

<br>Branch: <b>MAIN</b>


<br>CVS Tags: <b>HEAD</b>




<br>Changes since <b>1.1: +3 -3 lines</b>



<pre>update
</pre>

</td></tr></table>
<hr noshade>
<pre>
.if make(sandbox)
DESTDIR=destdir/${PROJECT_NAME}
.endif

XSLT=		xsltproc --param PEAK-XML-BASE-DIR "'${PWD}/'"
XSLDIR=		${HOME}/develop/project-xslt/stylesheets/
DIR2XML=	dir2xml --version-sort
POSTDIR=	sites/pmade.org/software/${PROJECT_NAME}
DESTDIR?=	${HOME}/${POSTDIR}
DLDIR=		${DESTDIR}/download
TARGETS=	${DESTDIR}/index.html ${DESTDIR}/resources.html ${DESTDIR}/news.html ${DESTDIR}/download.html

all: ${DESTDIR} ${TARGETS}
	chmod -R ugo+r ${DESTDIR} ${DESTDIR}/../images/

sync:
	rsync -Ccrlv --progress -e ssh ${DESTDIR}/* pmade.org:/usr/sites/pmade.org/htdocs/software/${PROJECT_NAME}/
	rsync -Ccrlv --progress -e ssh ${DESTDIR}/../images/* pmade.org:/usr/sites/pmade.org/htdocs/software/images/
	ssh pmade.org chmod -R ugo+r /usr/sites/pmade.org/htdocs/software

sandbox: all

clean:
	rm -rf destdir

${DESTDIR}::
	mkdir -p ${DESTDIR} ${DESTDIR}/../images ${DLDIR}
	cp -p ${XSLDIR}/../images/*.jpg ${DESTDIR}/../images/
	if [ -d ../images ] ; then \
		cp -p ../images/*.jpg ${DESTDIR}/../images/; \
	fi

${DESTDIR}/index.html: project.xml resources.xml news.xml ${XSLDIR}/project.xsl
	${XSLT} ${XSLDIR}/project.xsl project.xml &gt; ${DESTDIR}/index.html

${DESTDIR}/resources.html: resources.xml ${XSLDIR}/resources.xsl project.xml
	${XSLT} ${XSLDIR}/resources.xsl resources.xml &gt; ${DESTDIR}/resources.html

${DESTDIR}/news.html: news.xml ${XSLDIR}/news.xsl project.xml
	${XSLT} ${XSLDIR}/news.xsl news.xml &gt; ${DESTDIR}/news.html

${DESTDIR}/download.html::
	(cd ${DLDIR} ; ${DIR2XML}) &gt; dlfiles.xml
	${XSLT} ${XSLDIR}/download.xsl download.xml &gt; ${DESTDIR}/download.html
	rm -f dlfiles.xml

${XSLDIR}/project.xsl:		${XSLDIR}/master.xsl ${XSLDIR}/navigation.xsl ${XSLDIR}/formating.xsl ${XSLDIR}/linking.xsl
${XSLDIR}/resources.xsl:	${XSLDIR}/master.xsl ${XSLDIR}/navigation.xsl ${XSLDIR}/formating.xsl ${XSLDIR}/linking.xsl
${XSLDIR}/news.xsl:		${XSLDIR}/master.xsl ${XSLDIR}/navigation.xsl ${XSLDIR}/formating.xsl ${XSLDIR}/linking.xsl
</pre>

<hr noshade>
<table width="100%" border=0 cellpadding=0 cellspacing=0><tr>
<td align=left><address>webadmin at pmade dot org</address></td>
<td align=right>
Powered by<br><a href="http://viewcvs.sourceforge.net/">ViewCVS 0.9.2</a>
</td></tr></table>
</body></html>
