LANGUAGES=fr ru us
LANGFILES=$(LANGUAGES:=.xml)
KBDMAP=$(shell setxkbmap -query | while read key value; do echo -n "-$${key%:} $$value "; done)
SVG=$(LANGUAGES:=.svg)

all: $(SVG) style.svg florence.defs

$(LANGFILES):
	export PYTHONIOENCODING=UTF-8
	setxkbmap $(@:.xml=)
	python keymap_generator.py > $@
	setxkbmap $(KBDMAP)

style.svg: 
	cp ../svg/style.svg .

florence.defs:
	cp ../svg/florence.defs .

%.svg: %.xml
	python florence.py $< > $@

clean:
	rm -f $(LANGFILES) $(SVG) style.svg florence.defs
