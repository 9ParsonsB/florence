function loadDoc(file) {
	if (window.XMLHttpRequest) xhttp=new XMLHttpRequest();
	else xhttp=new ActiveXObject("Microsoft.XMLHTTP");
	xhttp.open("GET",file,false);
	xhttp.send();
	return xhttp.responseText;
} 

function loadXMLDoc(file) {
	if (window.XMLHttpRequest) xhttp=new XMLHttpRequest();
	else xhttp=new ActiveXObject("Microsoft.XMLHTTP");
	xhttp.open("GET",file,false);
	xhttp.send();
	return xhttp.responseXML;
} 

function onKeyPress(key) {
	var k = $("#h"+key.attr("id"));
	k.find(".shape").attr("class", k.find(".shape").attr("class").replace("hover", "selected"));
	$('#input').focus();
};

function latchorlock(modifier, lorl) {
	$(".key."+modifier).find(".shape").each(function() {
		$(this).attr("class", lorl + " " + $(this).attr("class"));
	});
	globalmod |= parseInt(modifier.substring(3));
}

function unlatchorunlock(modifier, lorl) {
	$(".key."+modifier).find(".shape").each(function() {
		$(this).attr("class", $(this).attr("class").replace(lorl + " ", ""));
	});
	globalmod &= (255 - parseInt(modifier.substring(3)));
}

function updateGlobalMod(oldgmod) {
	if (oldgmod != globalmod) {
		$("text.mod" + globalmod + ",use.mod" + globalmod).each(function(){
			$(this).parent().find("text.symbol-outline,text.symbol,use.symbol").hide();
			$(this).show();
		});
	}
}

function onKeyRelease(key) {
	var k = $("#h"+key.attr("id"));
	k.find(".shape").attr("class", k.find(".shape").attr("class").replace("selected", "hover"));
	var c = "";
	k.find("text.symbol,use.symbol").each(function() {
		if ((typeof $(this).attr("style") === "undefined") || ($(this).attr("style") != "display: none;")) {
			c = $(this).attr("class");
		}
	});

	if (c.search("BackSpace") > -1) {
		$("#input").backSpace();
	} else if (c.search("Return") > -1) {
		$("#input").insertAtCaret("\n");
	} else if (c.search("ISO_Left_Tab") > -1) {
		$("#input").leftTab();
	} else if (c.search("Tab") > -1) {
		$("#input").tab();
	} else if (c.search("Left") > -1) {
		$("#input").left();
	} else if (c.search("Right") > -1) {
		$("#input").right();
	} else if (c.search("Up") > -1) {
		$("#input").up();
	} else if (c.search("Down") > -1) {
		$("#input").down();
	} else if (c.search("Home") > -1) {
		$("#input").home();
	} else if (c.search("End") > -1) {
		$("#input").end();
	} else {
		var t = "";
		k.find("text.symbol").each(function() {
			if ((typeof $(this).attr("style") === "undefined") || ($(this).attr("style") != "display: none;")) {
				t = $(this).text()
			}
		});
		if (t.length == 1) $("#input").insertAtCaret(t);
	}

	var modifier = key.attr("class");
	var oldgmod = globalmod;
	if (modifier[3] == " ") {
		modifier = modifier.split(" ")[1]
		if (key.attr("class").search("locker") > -1) {
			if (-1 == key.find(".shape").attr("class").search("locked")) {
				latchorlock(modifier, "locked");
			} else {
				unlatchorunlock(modifier, "locked");
			}
		} else if (key.find(".shape").attr("class").search("locked") > -1) {
			unlatchorunlock(modifier, "locked");
		} else if (key.find(".shape").attr("class").search("latched") > -1) {
			unlatchorunlock(modifier, "latched");
			latchorlock(modifier, "locked");
		} else {
			latchorlock(modifier, "latched");
		}
	} else {
		$(".latched").each(function() {
			modifier = $(this).parent().attr("class").split(" ")[1];
			unlatchorunlock(modifier, "latched");
		});
	}
	updateGlobalMod(oldgmod);

	$('#input').focus();
};

var hoverKey = 0;
function onMouseOver(key) {
	if (key == 0) {
		if (hoverKey != 0) {
			hoverKey.hide();
			hoverKey = 0;
		}
	} else {
		k = $("#h"+key.attr("id"));
		if (k != hoverKey) {
			if (hoverKey != 0) {
				hoverKey.hide();
			}
			hoverKey = k;
			k.show();
		}
	}
};

var doc = loadDoc("us.svg");
var globalmod = "0";

$(document).ready(function() {
	$('#keyboard').html(doc);

	keyboardWidth = parseFloat($('#keyboard').find("svg").attr("width"));
	keyboardHeight = parseFloat($('#keyboard').find("svg").attr("height"));

	$('.key').each(function(){
		copy = $(this).clone();

		x = parseFloat(copy.attr('x'));
		y = parseFloat(copy.attr('y'));
		w = parseFloat(copy.attr('width'));
		h = parseFloat(copy.attr('height'));
		coords = $(this).parent().attr("transform").split("(")[1].split(")")[0].split(",")
		ox = parseFloat(coords[0]);
		oy = parseFloat(coords[1]);
		e = (-0.5*(x+(w/2.0))+ox);
		f = (-0.5*(y+(h/2.0))+oy);

		if ( (ox+x-(w/4.0)) < 0 ) e+= (w/4.0);
		else if ( (ox+x+(w*1.25)) > keyboardWidth ) e-= (w/4.0);
		if ( (oy+y-(h/4.0)) < 0 ) f+= (h/4.0);
		else if ( (oy+y+(h*1.25)) > keyboardHeight ) f-= (h/4.0);

		copy.attr("id", "h" + copy.attr("id"));
		copy.attr("transform", "matrix(1.5, 0, 0, 1.5, "+e.toString()+", "+f.toString()+")");
		copy.css("pointer-events", "none");
		copy.find(".shape").attr("class", copy.find(".shape").attr("class") + " hover");
		copy.hide();

		$(this).parent().parent().append(copy);

		$(this).mouseover(function(){onMouseOver($(this))});
		$(this).mouseout(function(){onMouseOver(0)});
		$(this).mousedown(function(){onKeyPress($(this))});
		$(this).mouseup(function(){onKeyRelease($(this))});
	});

});

