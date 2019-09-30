from __future__ import unicode_literals
from .errors import ParseError


class ParserStream(object):
    def __init__(self, string):
        self.string = string
        self.index = 0
        self.peek_offset = 0

    def get(self, offset):
        try:
            return self.string[offset]
        except IndexError:
            return None

<<<<<<< HEAD
    def char_at(self, offset):
        # When the cursor is at CRLF, return LF but don't move the cursor. The
        # cursor still points to the EOL position, which in this case is the
        # beginning of the compound CRLF sequence. This ensures slices of
        # [inclusive, exclusive) continue to work properly.
        if self.get(offset) == '\r' \
                and self.get(offset + 1) == '\n':
            return '\n'

        return self.get(offset)

    @property
    def current_char(self):
        return self.char_at(self.index)

    @property
    def current_peek(self):
        return self.char_at(self.index + self.peek_offset)
||||||| merged common ancestors
        if len(self.buf) == 0:
            self.ch = self.iter.next()
        else:
            self.ch = self.buf.pop(0)

        self.index += 1

        if self.ch is None:
            self.iter_end = True
            self.peek_end = True

        self.peek_index = self.index

        return self.ch
=======
    def char_at(self, offset):
        # When the cursor is at CRLF, return LF but don't move the cursor. The
        # cursor still points to the EOL position, which in this case is the
        # beginning of the compound CRLF sequence. This ensures slices of
        # [inclusive, exclusive) continue to work properly.
        if self.get(offset) == '\r' \
                and self.get(offset + 1) == '\n':
            return '\n'
>>>>>>> upstream-releases

<<<<<<< HEAD
    def next(self):
        self.peek_offset = 0
        # Skip over CRLF as if it was a single character.
        if self.get(self.index) == '\r' \
                and self.get(self.index + 1) == '\n':
            self.index += 1
        self.index += 1
        return self.get(self.index)
||||||| merged common ancestors
    def current(self):
        return self.ch
=======
        return self.get(offset)
>>>>>>> upstream-releases

<<<<<<< HEAD
    def peek(self):
        # Skip over CRLF as if it was a single character.
        if self.get(self.index + self.peek_offset) == '\r' \
                and self.get(self.index + self.peek_offset + 1) == '\n':
            self.peek_offset += 1
        self.peek_offset += 1
        return self.get(self.index + self.peek_offset)
||||||| merged common ancestors
    def current_is(self, ch):
        return self.ch == ch
=======
    @property
    def current_char(self):
        return self.char_at(self.index)
>>>>>>> upstream-releases

<<<<<<< HEAD
    def reset_peek(self, offset=0):
        self.peek_offset = offset

    def skip_to_peek(self):
        self.index += self.peek_offset
        self.peek_offset = 0


EOL = '\n'
EOF = None
SPECIAL_LINE_START_CHARS = ('}', '.', '[', '*')


class FluentParserStream(ParserStream):
    last_comment_zero_four_syntax = False

    def peek_blank_inline(self):
        start = self.index + self.peek_offset
        while self.current_peek == ' ':
            self.peek()
        return self.string[start:self.index + self.peek_offset]

    def skip_blank_inline(self):
        blank = self.peek_blank_inline()
        self.skip_to_peek()
        return blank

    def peek_blank_block(self):
        blank = ""
        while True:
            line_start = self.peek_offset
            self.peek_blank_inline()

            if self.current_peek == EOL:
                blank += EOL
                self.peek()
                continue

            if self.current_peek is EOF:
                # Treat the blank line at EOF as a blank block.
                return blank

            # Any other char; reset to column 1 on this line.
            self.reset_peek(line_start)
            return blank

    def skip_blank_block(self):
        blank = self.peek_blank_block()
        self.skip_to_peek()
        return blank

    def peek_blank(self):
        while self.current_peek in (" ", EOL):
            self.peek()

    def skip_blank(self):
        self.peek_blank()
        self.skip_to_peek()

    def expect_char(self, ch):
        if self.current_char == ch:
            self.next()
            return True

        raise ParseError('E0003', ch)

    def expect_line_end(self):
        if self.current_char is EOF:
            # EOF is a valid line end in Fluent.
            return True

        if self.current_char == EOL:
            self.next()
            return True

        # Unicode Character 'SYMBOL FOR NEWLINE' (U+2424)
        raise ParseError('E0003', '\u2424')

    def take_char(self, f):
        ch = self.current_char
        if ch is EOF:
            return EOF
        if f(ch):
            self.next()
            return ch
        return False

    def is_char_id_start(self, ch):
        if ch is EOF:
            return False
||||||| merged common ancestors
    def current_peek(self):
        if self.peek_end:
            return None

        diff = self.peek_index - self.index
=======
    @property
    def current_peek(self):
        return self.char_at(self.index + self.peek_offset)
>>>>>>> upstream-releases

<<<<<<< HEAD
        cc = ord(ch)
        return (cc >= 97 and cc <= 122) or \
               (cc >= 65 and cc <= 90)

    def is_identifier_start(self):
        return self.is_char_id_start(self.current_peek)
||||||| merged common ancestors
        if diff == 0:
            return self.ch
        return self.buf[diff - 1]

    def current_peek_is(self, ch):
        return self.current_peek() == ch
=======
    def next(self):
        self.peek_offset = 0
        # Skip over CRLF as if it was a single character.
        if self.get(self.index) == '\r' \
                and self.get(self.index + 1) == '\n':
            self.index += 1
        self.index += 1
        return self.get(self.index)
>>>>>>> upstream-releases

<<<<<<< HEAD
    def is_number_start(self):
        ch = self.peek() if self.current_char == '-' else self.current_char
        if ch is EOF:
            self.reset_peek()
            return False
||||||| merged common ancestors
    def peek(self):
        if self.peek_end:
            return None
=======
    def peek(self):
        # Skip over CRLF as if it was a single character.
        if self.get(self.index + self.peek_offset) == '\r' \
                and self.get(self.index + self.peek_offset + 1) == '\n':
            self.peek_offset += 1
        self.peek_offset += 1
        return self.get(self.index + self.peek_offset)
>>>>>>> upstream-releases

<<<<<<< HEAD
        cc = ord(ch)
        is_digit = cc >= 48 and cc <= 57
        self.reset_peek()
        return is_digit
||||||| merged common ancestors
        self.peek_index += 1
=======
    def reset_peek(self, offset=0):
        self.peek_offset = offset
>>>>>>> upstream-releases

<<<<<<< HEAD
    def is_char_pattern_continuation(self, ch):
        if ch is EOF:
            return False
||||||| merged common ancestors
        diff = self.peek_index - self.index
=======
    def skip_to_peek(self):
        self.index += self.peek_offset
        self.peek_offset = 0


EOL = '\n'
EOF = None
SPECIAL_LINE_START_CHARS = ('}', '.', '[', '*')


class FluentParserStream(ParserStream):

    def peek_blank_inline(self):
        start = self.index + self.peek_offset
        while self.current_peek == ' ':
            self.peek()
        return self.string[start:self.index + self.peek_offset]

    def skip_blank_inline(self):
        blank = self.peek_blank_inline()
        self.skip_to_peek()
        return blank

    def peek_blank_block(self):
        blank = ""
        while True:
            line_start = self.peek_offset
            self.peek_blank_inline()

            if self.current_peek == EOL:
                blank += EOL
                self.peek()
                continue

            if self.current_peek is EOF:
                # Treat the blank line at EOF as a blank block.
                return blank

            # Any other char; reset to column 1 on this line.
            self.reset_peek(line_start)
            return blank

    def skip_blank_block(self):
        blank = self.peek_blank_block()
        self.skip_to_peek()
        return blank

    def peek_blank(self):
        while self.current_peek in (" ", EOL):
            self.peek()

    def skip_blank(self):
        self.peek_blank()
        self.skip_to_peek()

    def expect_char(self, ch):
        if self.current_char == ch:
            self.next()
            return True

        raise ParseError('E0003', ch)

    def expect_line_end(self):
        if self.current_char is EOF:
            # EOF is a valid line end in Fluent.
            return True

        if self.current_char == EOL:
            self.next()
            return True

        # Unicode Character 'SYMBOL FOR NEWLINE' (U+2424)
        raise ParseError('E0003', '\u2424')

    def take_char(self, f):
        ch = self.current_char
        if ch is EOF:
            return EOF
        if f(ch):
            self.next()
            return ch
        return False

    def is_char_id_start(self, ch):
        if ch is EOF:
            return False
>>>>>>> upstream-releases

<<<<<<< HEAD
        return ch not in SPECIAL_LINE_START_CHARS
||||||| merged common ancestors
        if diff > len(self.buf):
            ch = self.iter.next()
            if ch is not None:
                self.buf.append(ch)
            else:
                self.peek_end = True
                return None
=======
        cc = ord(ch)
        return (cc >= 97 and cc <= 122) or \
               (cc >= 65 and cc <= 90)
>>>>>>> upstream-releases

<<<<<<< HEAD
    def is_value_start(self):
        # Inline Patterns may start with any char.
        return self.current_peek is not EOF and self.current_peek != EOL
||||||| merged common ancestors
        return self.buf[diff - 1]
=======
    def is_identifier_start(self):
        return self.is_char_id_start(self.current_peek)
>>>>>>> upstream-releases

<<<<<<< HEAD
    def is_value_continuation(self):
        column1 = self.peek_offset
        self.peek_blank_inline()
||||||| merged common ancestors
    def get_index(self):
        return self.index
=======
    def is_number_start(self):
        ch = self.peek() if self.current_char == '-' else self.current_char
        if ch is EOF:
            self.reset_peek()
            return False
>>>>>>> upstream-releases

<<<<<<< HEAD
        if self.current_peek == '{':
            self.reset_peek(column1)
            return True
||||||| merged common ancestors
    def get_peek_index(self):
        return self.peek_index
=======
        cc = ord(ch)
        is_digit = cc >= 48 and cc <= 57
        self.reset_peek()
        return is_digit
>>>>>>> upstream-releases

<<<<<<< HEAD
        if self.peek_offset - column1 == 0:
||||||| merged common ancestors
    def peek_char_is(self, ch):
        if self.peek_end:
=======
    def is_char_pattern_continuation(self, ch):
        if ch is EOF:
>>>>>>> upstream-releases
            return False

<<<<<<< HEAD
        if self.is_char_pattern_continuation(self.current_peek):
            self.reset_peek(column1)
            return True
||||||| merged common ancestors
        ret = self.peek()
=======
        return ch not in SPECIAL_LINE_START_CHARS
>>>>>>> upstream-releases

<<<<<<< HEAD
        return False
||||||| merged common ancestors
        self.peek_index -= 1
=======
    def is_value_start(self):
        # Inline Patterns may start with any char.
        return self.current_peek is not EOF and self.current_peek != EOL
>>>>>>> upstream-releases

<<<<<<< HEAD
    def is_next_line_zero_four_comment(self):
        if self.current_peek != EOL:
            return False
||||||| merged common ancestors
        return ret == ch
=======
    def is_value_continuation(self):
        column1 = self.peek_offset
        self.peek_blank_inline()
>>>>>>> upstream-releases

<<<<<<< HEAD
        is_comment = (self.peek(), self.peek()) == ('/', '/')
        self.reset_peek()
        return is_comment
||||||| merged common ancestors
    def reset_peek(self, pos=False):
        if pos:
            if pos < self.peek_index:
                self.peek_end = False
            self.peek_index = pos
        else:
            self.peek_index = self.index
            self.peek_end = self.iter_end
=======
        if self.current_peek == '{':
            self.reset_peek(column1)
            return True
>>>>>>> upstream-releases

<<<<<<< HEAD
    # -1 - any
    #  0 - comment
    #  1 - group comment
    #  2 - resource comment
    def is_next_line_comment(self, level=-1):
        if self.current_peek != EOL:
            return False
||||||| merged common ancestors
    def skip_to_peek(self):
        diff = self.peek_index - self.index

        for i in range(0, diff):
            self.ch = self.buf.pop(0)
=======
        if self.peek_offset - column1 == 0:
            return False

        if self.is_char_pattern_continuation(self.current_peek):
            self.reset_peek(column1)
            return True

        return False
>>>>>>> upstream-releases

<<<<<<< HEAD
        i = 0

        while (i <= level or (level == -1 and i < 3)):
            if self.peek() != '#':
                if i <= level and level != -1:
                    self.reset_peek()
                    return False
                break
            i += 1

        # The first char after #, ## or ###.
        if self.peek() in (' ', EOL):
            self.reset_peek()
            return True

        self.reset_peek()
        return False

    def is_variant_start(self):
        current_peek_offset = self.peek_offset
        if self.current_peek == '*':
            self.peek()
        if self.current_peek == '[' and self.peek() != '[':
            self.reset_peek(current_peek_offset)
            return True

        self.reset_peek(current_peek_offset)
        return False

    def is_attribute_start(self):
        return self.current_peek == '.'

    def skip_to_next_entry_start(self, junk_start):
        last_newline = self.string.rfind(EOL, 0, self.index)
        if junk_start < last_newline:
            # Last seen newline is _after_ the junk start. It's safe to rewind
            # without the risk of resuming at the same broken entry.
            self.index = last_newline

        while self.current_char:
            # We're only interested in beginnings of line.
            if self.current_char != EOL:
                self.next()
                continue

            # Break if the first char in this line looks like an entry start.
            first = self.next()
            if self.is_char_id_start(first) or first == '-' or first == '#':
                break

            # Syntax 0.4 compatibility
            peek = self.peek()
            self.reset_peek()
            if (first, peek) == ('/', '/') or (first, peek) == ('[', '['):
                break

    def take_id_start(self):
        if self.is_char_id_start(self.current_char):
            ret = self.current_char
            self.next()
            return ret
||||||| merged common ancestors
        self.index = self.peek_index
=======
    # -1 - any
    #  0 - comment
    #  1 - group comment
    #  2 - resource comment
    def is_next_line_comment(self, level=-1):
        if self.current_peek != EOL:
            return False

        i = 0

        while (i <= level or (level == -1 and i < 3)):
            if self.peek() != '#':
                if i <= level and level != -1:
                    self.reset_peek()
                    return False
                break
            i += 1

        # The first char after #, ## or ###.
        if self.peek() in (' ', EOL):
            self.reset_peek()
            return True

        self.reset_peek()
        return False

    def is_variant_start(self):
        current_peek_offset = self.peek_offset
        if self.current_peek == '*':
            self.peek()
        if self.current_peek == '[' and self.peek() != '[':
            self.reset_peek(current_peek_offset)
            return True

        self.reset_peek(current_peek_offset)
        return False

    def is_attribute_start(self):
        return self.current_peek == '.'

    def skip_to_next_entry_start(self, junk_start):
        last_newline = self.string.rfind(EOL, 0, self.index)
        if junk_start < last_newline:
            # Last seen newline is _after_ the junk start. It's safe to rewind
            # without the risk of resuming at the same broken entry.
            self.index = last_newline

        while self.current_char:
            # We're only interested in beginnings of line.
            if self.current_char != EOL:
                self.next()
                continue

            # Break if the first char in this line looks like an entry start.
            first = self.next()
            if self.is_char_id_start(first) or first == '-' or first == '#':
                break

            # Syntax 0.4 compatibility
            peek = self.peek()
            self.reset_peek()
            if (first, peek) == ('/', '/') or (first, peek) == ('[', '['):
                break

    def take_id_start(self):
        if self.is_char_id_start(self.current_char):
            ret = self.current_char
            self.next()
            return ret
>>>>>>> upstream-releases

        raise ParseError('E0004', 'a-zA-Z')

    def take_id_char(self):
        def closure(ch):
            cc = ord(ch)
            return ((cc >= 97 and cc <= 122) or
                    (cc >= 65 and cc <= 90) or
                    (cc >= 48 and cc <= 57) or
                    cc == 95 or cc == 45)
        return self.take_char(closure)

    def take_digit(self):
        def closure(ch):
            cc = ord(ch)
            return (cc >= 48 and cc <= 57)
        return self.take_char(closure)

    def take_hex_digit(self):
        def closure(ch):
            cc = ord(ch)
            return (
                (cc >= 48 and cc <= 57)   # 0-9
                or (cc >= 65 and cc <= 70)  # A-F
                or (cc >= 97 and cc <= 102))  # a-f
        return self.take_char(closure)
