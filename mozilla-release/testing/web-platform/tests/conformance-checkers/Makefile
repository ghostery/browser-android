HTML2MARKDOWN=html2text
PERL=perl
PERLFLAGS=
FMT=fmt
FMTFLAGS=-80
EXPAND=expand
EXPANDFLAGS=
GIT=git
GITFLAGS=
PYTHON=python
PYTHONFLAGS=
CURL=curl
CURLFLAGS=
JAVA=java
JAVAFLAGS=
VNU_TEST_REPO=git@github.com:validator/tests.git
ITS_REPO=git@github.com:w3c/its-2.0-testsuite-inputdata.git
.PHONY: .FORCE

all: README.md messages.json

README.md: index.html
	$(HTML2MARKDOWN) $(HTML2MARKDOWNFLAGS) $< \
	    | $(PERL) $(PERLFLAGS) -pe 'undef $$/; s/(\s+\n)+/\n\n/g' \
	    | $(PERL) $(PERLFLAGS) -pe 'undef $$/; s/(\n\n\n)+/\n/g' \
	    | $(FMT) $(FMTFLAGS) \
	    | $(PERL) $(PERLFLAGS) -pe 'undef $$/; s/ +(\[[0-9]+\]:)\n +/\n   $$1 /g' \
	    | $(EXPAND) $(EXPANDFLAGS) > $@

messages.json: .FORCE
<<<<<<< HEAD
	$(CURL) $(CURLFLAGS) -O \
	  https://sideshowbarker.net/nightlies/jar/vnu.jar
	$(JAVA) $(JAVAFLAGS) -cp vnu.jar nu.validator.client.TestRunner \
	  --ignore=html-its --write-messages $@
||||||| merged common ancestors
=======
	$(CURL) $(CURLFLAGS) -O -L \
	  https://github.com/validator/validator/releases/download/jar/vnu.jar
	$(JAVA) $(JAVAFLAGS) -cp vnu.jar nu.validator.client.TestRunner \
	  --ignore=html-its --write-messages $@
>>>>>>> upstream-releases
	$(PYTHON) $(PYTHONFLAGS) -mjson.tool $@ > $@.tmp
	mv $@.tmp $@

<<<<<<< HEAD
test: .FORCE
	$(CURL) $(CURLFLAGS) -O \
	  https://sideshowbarker.net/nightlies/jar/vnu.jar
	$(JAVA) $(JAVAFLAGS) -cp vnu.jar nu.validator.client.TestRunner \
	  --ignore=html-its messages.json

||||||| merged common ancestors
=======
test: .FORCE
	$(CURL) $(CURLFLAGS) -O -L \
	  https://github.com/validator/validator/releases/download/jar/vnu.jar
	$(JAVA) $(JAVAFLAGS) -cp vnu.jar nu.validator.client.TestRunner \
	  --ignore=html-its messages.json

>>>>>>> upstream-releases
push:
	cd .. \
	  && git push $(VNU_TEST_REPO) `git subtree split -P conformance-checkers`:master --force \
	  && cd -

its-push:
	cd ..\
	  && $(GIT) subtree push -P conformance-checkers/html-its/ $(ITS_REPO) master \
	  && cd -

its-pull:
	cd .. \
	  && $(GIT) pull -s subtree $(ITS_REPO) master \
	  && cd -
