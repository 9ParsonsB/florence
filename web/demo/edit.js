var offset = -1;
$.fn.insertAtCaret = function(myValue) {
	return this.each(function() {
		var me = this;
		if (document.selection) { // IE
			me.focus();
			sel = document.selection.createRange();
			sel.text = myValue;
			me.focus();
		} else if (me.selectionStart || me.selectionStart == '0') { // Real browsers
			var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
			me.value = me.value.substring(0, startPos) + myValue + me.value.substring(endPos, me.value.length);
			me.focus();
			me.selectionStart = startPos + myValue.length;
			me.selectionEnd = startPos + myValue.length;
			me.scrollTop = scrollTop;
		} else {
			me.value += myValue;
			me.focus();
		}
		offset = -1;
	});
};

$.fn.left = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		me.selectionStart = startPos - 1;
		me.selectionEnd = startPos - 1;
		me.scrollTop = scrollTop;
		offset = -1;
	});
};

$.fn.right = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		me.selectionStart = endPos + 1;
		me.selectionEnd = endPos + 1;
		me.scrollTop = scrollTop;
		offset = -1;
	});
};

$.fn.up = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		var beginStr = me.value.substring(0, startPos);
		var index = beginStr.lastIndexOf("\n");
		if (index > -1) {
			if (offset == -1 || me.value.substring(startPos,startPos+1) != '\n') {
				offset = startPos - index;
			}
			beginStr = me.value.substring(0, index);
			var index2 = beginStr.lastIndexOf("\n");
			if ( (index2 + offset) < index ) me.selectionStart = index2 + offset;
			else me.selectionStart = index;
			me.selectionEnd = me.selectionStart;
			me.scrollTop = scrollTop;
		}
	});
};

$.fn.down = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		var beginStr = me.value.substring(0, endPos);
		var index = beginStr.lastIndexOf("\n");
		if (offset == -1 || me.value.substring(endPos,endPos+1) != '\n') {
			offset = endPos + 1;
			if (index > -1) {
				offset = endPos - index;
			}
		}
		var endStr = me.value.substring(endPos);
		var index = endStr.search("\n");
		if (index > -1) {
			index2 = endStr.substring(index + 1, index + offset).search("\n");
			if (index2 > -1) {
				me.selectionStart = endPos + index + index2 + 1;
			} else me.selectionStart = endPos + index + offset;
			me.selectionEnd = me.selectionStart;
			me.scrollTop = scrollTop;
		}
	});
};

$.fn.backSpace = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		if (startPos == endPos) {
			me.value = me.value.substring(0, startPos-1) + me.value.substring(endPos, me.value.length);
			me.focus();
			me.selectionStart = startPos - 1;
			me.selectionEnd = startPos - 1;
		} else {
			me.value = me.value.substring(0, startPos) + me.value.substring(endPos, me.value.length);
			me.focus();
			me.selectionStart = startPos;
			me.selectionEnd = startPos;
		}
		me.scrollTop = scrollTop;
		offset = -1;
	});
};

$.fn.tab = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		if (startPos == endPos) {
			$(me).insertAtCaret("\t");
		} else {
			var m = me.value.substring(startPos, endPos).match(/\n/g); 
			var count = 1
			if (m !== null) count += m.length; 
			var beginStr = me.value.substring(0, startPos);
			if (me.value[startPos] == '\n') {
				count -= 1;
			} else if (beginStr.search("\n") > - 1) {
				var index = beginStr.lastIndexOf("\n");
				beginStr = me.value.substring(0, index+1) + "\t" +  me.value.substring(index+1, startPos);
			} else {
				beginStr = "\t" + me.value.substring(0, startPos);
			}
			me.value = beginStr +
			      me.value.substring(startPos, endPos).replace(/\n/g, "\n\t") +
			      me.value.substring(endPos, me.value.length);
			me.focus();
			if (me.value[startPos] != '\t') startPos += 1;
			me.selectionStart = startPos;
			me.selectionEnd = endPos+count;
			me.scrollTop = scrollTop;
		}
		offset = -1;
	});
};

$.fn.leftTab = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		if (startPos == endPos) {
			var beginStr = me.value.substring(0, startPos);
			var index = -1;
			if (beginStr.search("\n") > - 1) {
				index = beginStr.lastIndexOf("\n\t");
				if (index > -1) index += 1;
			} else {
				index = (beginStr.substring(0,1) == '\t'?0:-1)
			}
			if (index > -1) {
				beginStr = me.value.substring(0, index) + me.value.substring(index+1, startPos);
				startPos -= 1;
			}
			me.value = beginStr + me.value.substring(endPos, me.value.length);
			me.focus();
			me.selectionStart = startPos;
			me.selectionEnd = startPos;
		} else {
			var m = me.value.substring(startPos, endPos).match(/\n\t/g);
			var count = 0
			if (m !== null) count += m.length; 
			var beginStr = me.value.substring(0, startPos);
			var offset = -1;
			if (beginStr.search("\n") > -1) {
				var index = beginStr.lastIndexOf("\n");
				if (((index+1) == startPos) && (me.value[startPos] == '\t')) {
					startPos += 1;
					offset += 1;
				} else if (me.value[index+1] == '\t') {
					beginStr = me.value.substring(0, index+1) + me.value.substring(index+2, startPos);
					offset += 1;
				}
			} else {
				if (beginStr[0] == '\t') {
					beginStr = me.value.substring(1, startPos);
					offset += 1;
				}
			}
			me.value = beginStr +
			      me.value.substring(startPos, endPos).replace(/\n\t/g, "\n") +
			      me.value.substring(endPos, me.value.length);
			me.focus();
			me.selectionStart = startPos - offset;
			me.selectionEnd = endPos - count - offset;
		}
		me.scrollTop = scrollTop;
		offset = -1;
	});
};

$.fn.home = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		var beginStr = me.value.substring(0, startPos);
		var index = beginStr.lastIndexOf("\n");
		if (index > -1) {
			me.selectionStart = index + 1;
		} else {
			me.selectionStart = 0;
		}
		me.selectionEnd = me.selectionStart;
		me.scrollTop = scrollTop;
	});
};

$.fn.end = function() {
	return this.each(function() {
		var me = this;
		var startPos = me.selectionStart, endPos = me.selectionEnd, scrollTop = me.scrollTop;
		me.focus();
		var endStr = me.value.substring(endPos);
		var index = endStr.search("\n");
		if (index > -1) {
			me.selectionStart = endPos + index;
		} else {
			me.selectionStart = me.value.length;
		}
		me.selectionEnd = me.selectionStart;
		me.scrollTop = scrollTop;
	});
};

