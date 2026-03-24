# ----------------------------
# Makefile Options
# ----------------------------

NAME = WEB1999
ICON = ie16.png
DESCRIPTION = "1999-style web browser"
COMPRESSED = YES
ARCHIVED = NO

EXTRA_DEFINES ?=
CFLAGS = -Wall -Wextra -Oz $(EXTRA_DEFINES)
CXXFLAGS = -Wall -Wextra -Oz $(EXTRA_DEFINES)

EXTRA_CLEAN = $(MAKE) $(addprefix clean-,readme release)

GIT_VERSION := $(shell git describe --always --dirty)

# ----------------------------

include $(shell cedev-config --makefile)

.PHONY: testvis
testvis:
	$(MAKE) debug NAME=W9TESTV DESCRIPTION="WEB1999 test visualizer" EXTRA_DEFINES=-DTESTVIS

README.html: README.md scripts/readme-extra.css $(wildcard img/*)
	pandoc -f markdown+auto_identifiers -t html \
		--shift-heading-level-by=-1 \
		--metadata=document-css=true --css scripts/readme-extra.css \
		--metadata=subtitle:"$(GIT_VERSION)" \
		--metadata=date:"$(shell date "+%B %-d, %Y")" \
		--metadata=toc-title=Contents \
		--self-contained --toc --toc-depth=2 $< -o $@
README.txt: README.md scripts/remove-images.lua
	pandoc -f markdown -t markdown \
		--reference-links --reference-location=section \
		--lua-filter=scripts/remove-images.lua \
		--markdown-headings=setext $< -o $@
.PHONY: readme clean-readme
readme: README.html README.txt
clean-readme:
	$(call RM,README.html README.txt)


WEB1999-$(GIT_VERSION).zip: bin/WEB1999.8xp README.html README.txt LICENSE.txt
	git archive --format=zip -o $@ $(addprefix --add-file=,$^) --prefix=source/ HEAD

.PHONY: release clean-release
release: WEB1999-$(GIT_VERSION).zip
clean-release:
	$(call RM,$(wildcard WEB1999-*.zip))
