####
# Variables set by user
XML_INPUT?= 		manual.xml
PROJECT_NAME?=		unknown-project
INSTALL_TO?=		${HOME}/sites/pmade.org/software/${PROJECT_NAME}/download/documentation
INSTALL_DIR?=		${XML_INPUT:.xml=}

####
# General options
CSS_FILE=		docbook.css
CSS_SOURCE=		${HOME}/develop/project-xslt/docbook/${CSS_FILE}
####


####
# XSLT options
XSLTPROC= 		xsltproc --xinclude
XSLT_PARAMS= 		--stringparam section.autolabel 1 \
			--stringparam  section.label.includes.component.label 1
####


####
# Options for all-in-one HTML generation
HTML_DEST= 		one-html
HTML_OUT= 		${HTML_DEST}/${XML_INPUT:.xml=.html}
XSL_DOCKBOOK_HTML= 	/usr/local/share/xsl/docbook/html/docbook.xsl
HTML_PARAMS=		--stringparam html.stylesheet ${CSS_FILE}
####


####
# Options for one-page-per-chapter HTML generation
SEP_HTML_DEST=		sep-html
SEP_HTML_OUT=		${SEP_HTML_DEST}/index.html
XSL_DOCKBOOK_SEP_HTML= 	/usr/local/share/xsl/docbook/html/chunk.xsl
SEP_HTML_PARAMS=	--stringparam base.dir ${SEP_HTML_DEST}/ \
			--stringparam chunk.fast 1 \
			--stringparam use.id.as.filename 1
####


####
# Options for PDF output
PDF_DEST=		postscript
PDF_OUT=		${PDF_DEST}/${XML_INPUT:.xml=.pdf}
XSL_DOCKBOOK_PDF=	/usr/local/share/xsl/docbook/fo/docbook.xsl
PDF_PARAMS=		--stringparam  double.sided 1 \
			--stringparam  passivetex.extensions  1 \
			--stringparam insert.xref.page.number 1
####


####
# Default Target and aliases
all: ${HTML_OUT} ${SEP_HTML_OUT} ${PDF_OUT}
html: ${HTML_OUT} ${SEP_HTML_OUT}
one-html: ${HTML_OUT}
sep-html: ${SEP_HTML_OUT}
pdf: ${PDF_OUT}
####

####
# Generate HTML output
${HTML_OUT}: ${XML_INPUT}
	mkdir -p ${HTML_DEST}
	cp ${CSS_SOURCE} ${HTML_DEST}/${CSS_FILE}
	${XSLTPROC} ${XSLT_PARAMS} ${HTML_PARAMS} --output ${HTML_OUT} ${XSL_DOCKBOOK_HTML} ${XML_INPUT}
####


####
# Generate SEP_HTML output
${SEP_HTML_OUT}: ${XML_INPUT}
	mkdir -p ${SEP_HTML_DEST}
	cp ${CSS_SOURCE} ${SEP_HTML_DEST}/${CSS_FILE}
	${XSLTPROC} ${XSLT_PARAMS} ${HTML_PARAMS} ${SEP_HTML_PARAMS} ${XSL_DOCKBOOK_SEP_HTML} ${XML_INPUT}
####


####
# Generate PDF output
${PDF_OUT}: ${XML_INPUT}
	mkdir -p ${PDF_DEST}
	${XSLTPROC} ${XSLT_PARAMS} ${PDF_PARAMS} --output ${PDF_DEST}/${XML_INPUT:.xml=.fo} ${XSL_DOCKBOOK_PDF} ${XML_INPUT}
	(cd ${PDF_DEST}; pdftex --interaction nonstopmode "&pdfxmltex" ${XML_INPUT:.xml=.fo})
	(cd ${PDF_DEST}; pdftex --interaction nonstopmode "&pdfxmltex" ${XML_INPUT:.xml=.fo})

####
# Install generated documentation
install: all
	mkdir -p ${INSTALL_TO}/${INSTALL_DIR}
	cp -p ${HTML_OUT} ${INSTALL_TO}/${INSTALL_DIR}/
	cp -p ${HTML_DEST}/${CSS_FILE} ${INSTALL_TO}/${INSTALL_DIR}/
	cp -pr ${SEP_HTML_DEST} ${INSTALL_TO}/${INSTALL_DIR}/
	cp -pr ${PDF_OUT} ${INSTALL_TO}/${INSTALL_DIR}/
	

#### 
# Clean things up
clean:
	rm -rf ${HTML_DEST} ${SEP_HTML_DEST} ${PDF_DEST}
####
