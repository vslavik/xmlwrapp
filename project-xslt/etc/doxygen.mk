VERSION_FILE?=${PROJECT_PATH}/docs/VERSION
VERSION!=head -1 ${VERSION_FILE} | perl -pe 's/\s+.*$$//'
XML_DIR=${PROJECT_PATH}/docs/project/

PROJECT_XSLT?=${HOME}/develop/project-xslt
DOXYGEN_XSL=${PROJECT_XSLT}/stylesheets/doxygen.xsl
PROJECT_XML=${XML_DIR}project.xml
LOGO=${PROJECT_XSLT}/images/${PROJECT_NAME}-logo.jpg
DOXYGEN_LOGO=${PROJECT_XSLT}/images/doxygen.jpg
TMP_DOXYFILE=${PWD}/doxyfile.tmp

INSTALL_DIR=${HOME}/sites/pmade.org/software/${PROJECT_NAME}/download/documentation

all: online tarball

online:
	xsltproc --param PEAK-XML-BASE-DIR "'${XML_DIR}'" --param image-dir "''" --param html-dir "'/software/${PROJECT_NAME}/'" ${DOXYGEN_XSL} ${PROJECT_XML} > tmp.html
	cp tmp.html header.html
	cp tmp.html footer.html
	rm tmp.html
	perl -i -ne 'if (/<!--SPLIT-ME-->/) { $$x = 1; } print unless $$x' header.html
	perl -i -ne 'if (/<!--SPLIT-ME-->/) { $$x = 1; next; } print if $$x' footer.html
	cp ${PROJECT_PATH}/${DOXYGEN_DIR}/doxyfile ${TMP_DOXYFILE}
	perl -i -pe 'if (/^HTML_HEADER/) { $$_ = "HTML_HEADER = ${PWD}/header.html"; } if (/^HTML_FOOTER/) { $$_ = "HTML_FOOTER = ${PWD}/footer.html"; }' ${TMP_DOXYFILE}
	perl -i -pe 's/^\s*(PROJECT_NUMBER\s*=).*$$/$${1}${VERSION}/' ${TMP_DOXYFILE}
	(cd ${PROJECT_PATH}/${DOXYGEN_DIR}; doxygen ${TMP_DOXYFILE})
	mv ${PROJECT_PATH}/${DOXYGEN_DIR}/html .
	cp ${LOGO} html/
	cp ${DOXYGEN_LOGO} html/
	rm header.html footer.html ${TMP_DOXYFILE}
	mv html doxygen

tarball:
	cp ${PROJECT_PATH}/${DOXYGEN_DIR}/doxyfile ${TMP_DOXYFILE}
	perl -i -pe 's/^\s*(PROJECT_NUMBER\s*=).*$$/$${1}${VERSION}/' ${TMP_DOXYFILE}
	(cd ${PROJECT_PATH}/${DOXYGEN_DIR}; doxygen ${TMP_DOXYFILE})
	mv ${PROJECT_PATH}/${DOXYGEN_DIR}/html ${PROJECT_NAME}-api
	tar czvf ${PROJECT_NAME}-api.tar.gz ${PROJECT_NAME}-api
	rm -rf ${PROJECT_NAME}-api ${TMP_DOXYFILE}

clean:
	rm -rf doxygen ${PROJECT_NAME}-api.tar.gz

install: all
	mkdir -p ${INSTALL_DIR}
	cp -p ${PROJECT_NAME}-api.tar.gz ${INSTALL_DIR}/
	cp -rp doxygen ${INSTALL_DIR}/

