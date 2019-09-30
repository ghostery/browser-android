from __future__ import unicode_literals
import re
from . import ast
from .stream import EOF, EOL, FluentParserStream
from .errors import ParseError


def with_span(fn):
    def decorated(self, ps, *args, **kwargs):
        if not self.with_spans:
            return fn(self, ps, *args, **kwargs)

        start = ps.index
        node = fn(self, ps, *args, **kwargs)

        # Don't re-add the span if the node already has it. This may happen
        # when one decorated function calls another decorated function.
        if node.span is not None:
            return node

        end = ps.index
        node.add_span(start, end)
        return node

    return decorated


class FluentParser(object):
    def __init__(self, with_spans=True):
        self.with_spans = with_spans

    def parse(self, source):
        ps = FluentParserStream(source)
        ps.skip_blank_block()

        entries = []
        last_comment = None

        while ps.current_char:
            entry = self.get_entry_or_junk(ps)
            blank_lines = ps.skip_blank_block()

            # Regular Comments require special logic. Comments may be attached
            # to Messages or Terms if they are followed immediately by them.
            # However they should parse as standalone when they're followed by
            # Junk. Consequently, we only attach Comments once we know that the
            # Message or the Term parsed successfully.
            if isinstance(entry, ast.Comment) and len(blank_lines) == 0 \
                    and ps.current_char:
                # Stash the comment and decide what to do with it
                # in the next pass.
                last_comment = entry
                continue

            if last_comment is not None:
                if isinstance(entry, (ast.Message, ast.Term)):
                    entry.comment = last_comment
                    if self.with_spans:
                        entry.span.start = entry.comment.span.start
                else:
                    entries.append(last_comment)
                # In either case, the stashed comment has been dealt with;
                # clear it.
                last_comment = None

            entries.append(entry)

        res = ast.Resource(entries)

        if self.with_spans:
            res.add_span(0, ps.index)

        return res

    def parse_entry(self, source):
        """Parse the first Message or Term in source.

        Skip all encountered comments and start parsing at the first Mesage
        or Term start. Return Junk if the parsing is not successful.

        Preceding comments are ignored unless they contain syntax errors
        themselves, in which case Junk for the invalid comment is returned.
        """
        ps = FluentParserStream(source)
        ps.skip_blank_block()

        while ps.current_char == '#':
            skipped = self.get_entry_or_junk(ps)
            if isinstance(skipped, ast.Junk):
                # Don't skip Junk comments.
                return skipped
            ps.skip_blank_block()

        return self.get_entry_or_junk(ps)

    def get_entry_or_junk(self, ps):
        entry_start_pos = ps.index

        try:
            entry = self.get_entry(ps)
            ps.expect_line_end()
            return entry
        except ParseError as err:
            error_index = ps.index
            ps.skip_to_next_entry_start(entry_start_pos)
            next_entry_start = ps.index
            if next_entry_start < error_index:
                # The position of the error must be inside of the Junk's span.
                error_index = next_entry_start

            # Create a Junk instance
            slice = ps.string[entry_start_pos:next_entry_start]
            junk = ast.Junk(slice)
            if self.with_spans:
                junk.add_span(entry_start_pos, next_entry_start)
            annot = ast.Annotation(err.code, err.args, err.message)
            annot.add_span(error_index, error_index)
            junk.add_annotation(annot)
            return junk

    def get_entry(self, ps):
        if ps.current_char == '#':
            return self.get_comment(ps)

<<<<<<< HEAD
        if ps.current_char == '/':
            return self.get_zero_four_style_comment(ps)

        if ps.current_char == '[':
            return self.get_group_comment_from_section(ps)

        if ps.current_char == '-':
||||||| merged common ancestors
        if ps.current_is('/'):
            return self.get_zero_four_style_comment(ps)

        if ps.current_is('['):
            return self.get_group_comment_from_section(ps)

        if ps.current_is('-'):
=======
        if ps.current_char == '-':
>>>>>>> upstream-releases
            return self.get_term(ps)

        if ps.is_identifier_start():
            return self.get_message(ps)

        raise ParseError('E0002')

    @with_span
<<<<<<< HEAD
    def get_zero_four_style_comment(self, ps):
        ps.expect_char('/')
        ps.expect_char('/')
        ps.take_char(lambda x: x == ' ')

        content = ''

        while True:
            ch = ps.take_char(lambda x: x != EOL)
            while ch:
                content += ch
                ch = ps.take_char(lambda x: x != EOL)

            if ps.is_next_line_zero_four_comment():
                content += ps.current_char
                ps.next()
                ps.expect_char('/')
                ps.expect_char('/')
                ps.take_char(lambda x: x == ' ')
            else:
                break

        # Comments followed by Sections become GroupComments.
        if ps.peek() == '[':
            ps.skip_to_peek()
            self.get_group_comment_from_section(ps)
            return ast.GroupComment(content)

        ps.reset_peek()
        ps.last_comment_zero_four_syntax = True
        return ast.Comment(content)

    @with_span
||||||| merged common ancestors
    def get_zero_four_style_comment(self, ps):
        ps.expect_char('/')
        ps.expect_char('/')
        ps.take_char(lambda x: x == ' ')

        content = ''

        while True:
            ch = ps.take_char(lambda x: x != '\n')
            while ch:
                content += ch
                ch = ps.take_char(lambda x: x != '\n')

            if ps.is_peek_next_line_zero_four_style_comment():
                content += ps.current()
                ps.next()
                ps.expect_char('/')
                ps.expect_char('/')
                ps.take_char(lambda x: x == ' ')
            else:
                break

        # Comments followed by Sections become GroupComments.
        ps.peek()
        if ps.current_peek_is('['):
            ps.skip_to_peek()
            self.get_group_comment_from_section(ps)
            return ast.GroupComment(content)

        ps.reset_peek()
        ps.last_comment_zero_four_syntax = True
        return ast.Comment(content)

    @with_span
=======
>>>>>>> upstream-releases
    def get_comment(self, ps):
        # 0 - comment
        # 1 - group comment
        # 2 - resource comment
        level = -1
        content = ''

        while True:
            i = -1
            while ps.current_char == '#' \
                    and (i < (2 if level == -1 else level)):
                ps.next()
                i += 1

            if level == -1:
                level = i

            if ps.current_char != EOL:
                ps.expect_char(' ')
                ch = ps.take_char(lambda x: x != EOL)
                while ch:
                    content += ch
                    ch = ps.take_char(lambda x: x != EOL)

            if ps.is_next_line_comment(level=level):
                content += ps.current_char
                ps.next()
            else:
                break

        if level == 0:
            return ast.Comment(content)
        elif level == 1:
            return ast.GroupComment(content)
        elif level == 2:
            return ast.ResourceComment(content)

    @with_span
<<<<<<< HEAD
    def get_group_comment_from_section(self, ps):
        def until_closing_bracket_or_eol(ch):
            return ch not in (']', EOL)

        ps.expect_char('[')
        ps.expect_char('[')
        while ps.take_char(until_closing_bracket_or_eol):
            pass
        ps.expect_char(']')
        ps.expect_char(']')

        # A Section without a comment is like an empty Group Comment.
        # Semantically it ends the previous group and starts a new one.
        return ast.GroupComment('')

    @with_span
||||||| merged common ancestors
    def get_group_comment_from_section(self, ps):
        ps.expect_char('[')
        ps.expect_char('[')

        ps.skip_inline_ws()

        self.get_variant_name(ps)

        ps.skip_inline_ws()

        ps.expect_char(']')
        ps.expect_char(']')

        # A Section without a comment is like an empty Group Comment.
        # Semantically it ends the previous group and starts a new one.
        return ast.GroupComment('')

    @with_span
=======
>>>>>>> upstream-releases
    def get_message(self, ps):
        id = self.get_identifier(ps)
<<<<<<< HEAD
        ps.skip_blank_inline()

        # XXX Syntax 0.4 compat
        if ps.current_char == '=':
            ps.next()
            value = self.maybe_get_pattern(ps)
        else:
            value = None
||||||| merged common ancestors

        ps.skip_inline_ws()
        pattern = None

        # XXX Syntax 0.4 compat
        if ps.current_is('='):
            ps.next()

            if ps.is_peek_value_start():
                ps.skip_indent()
                pattern = self.get_pattern(ps)
            else:
                ps.skip_inline_ws()

        if ps.is_peek_next_line_attribute_start():
            attrs = self.get_attributes(ps)
        else:
            attrs = None
=======
        ps.skip_blank_inline()
        ps.expect_char('=')

        value = self.maybe_get_pattern(ps)
        attrs = self.get_attributes(ps)
>>>>>>> upstream-releases

<<<<<<< HEAD
        attrs = self.get_attributes(ps)

        if value is None and len(attrs) == 0:
||||||| merged common ancestors
        if pattern is None and attrs is None:
=======
        if value is None and len(attrs) == 0:
>>>>>>> upstream-releases
            raise ParseError('E0005', id.name)

        return ast.Message(id, value, attrs)

    @with_span
    def get_term(self, ps):
        ps.expect_char('-')
        id = self.get_identifier(ps)

        ps.skip_blank_inline()
        ps.expect_char('=')

<<<<<<< HEAD
        # Syntax 0.8 compat: VariantLists are supported but deprecated. They
        # can only be found as values of Terms. Nested VariantLists are not
        # allowed.
        value = self.maybe_get_variant_list(ps) or self.maybe_get_pattern(ps)
        if value is None:
||||||| merged common ancestors
        if ps.is_peek_value_start():
            ps.skip_indent()
            value = self.get_value(ps)
        else:
=======
        value = self.maybe_get_pattern(ps)
        if value is None:
>>>>>>> upstream-releases
            raise ParseError('E0006', id.name)

        attrs = self.get_attributes(ps)
        return ast.Term(id, value, attrs)

    @with_span
    def get_attribute(self, ps):
        ps.expect_char('.')

        key = self.get_identifier(ps)

        ps.skip_blank_inline()
        ps.expect_char('=')

<<<<<<< HEAD
        value = self.maybe_get_pattern(ps)
        if value is None:
            raise ParseError('E0012')

        return ast.Attribute(key, value)
||||||| merged common ancestors
        if ps.is_peek_value_start():
            ps.skip_indent()
            value = self.get_pattern(ps)
            return ast.Attribute(key, value)
=======
        value = self.maybe_get_pattern(ps)
        if value is None:
            raise ParseError('E0012')
>>>>>>> upstream-releases

<<<<<<< HEAD
||||||| merged common ancestors
        raise ParseError('E0012')
=======
        return ast.Attribute(key, value)
>>>>>>> upstream-releases

    def get_attributes(self, ps):
        attrs = []
        ps.peek_blank()

        while ps.is_attribute_start():
            ps.skip_to_peek()
            attr = self.get_attribute(ps)
            attrs.append(attr)
<<<<<<< HEAD
            ps.peek_blank();
||||||| merged common ancestors
=======
            ps.peek_blank()
>>>>>>> upstream-releases

        return attrs

    @with_span
    def get_identifier(self, ps):
        name = ps.take_id_start()
        ch = ps.take_id_char()
        while ch:
            name += ch
            ch = ps.take_id_char()

        return ast.Identifier(name)

    def get_variant_key(self, ps):
        ch = ps.current_char

        if ch is EOF:
            raise ParseError('E0013')

        cc = ord(ch)
        if ((cc >= 48 and cc <= 57) or cc == 45):  # 0-9, -
            return self.get_number(ps)

        return self.get_identifier(ps)

    @with_span
    def get_variant(self, ps, has_default):
        default_index = False

        if ps.current_char == '*':
            if has_default:
                raise ParseError('E0015')
            ps.next()
            default_index = True

        ps.expect_char('[')
        ps.skip_blank()

        key = self.get_variant_key(ps)

        ps.skip_blank()
        ps.expect_char(']')

<<<<<<< HEAD
        value = self.maybe_get_pattern(ps)
        if value is None:
            raise ParseError('E0012')

        return ast.Variant(key, value, default_index)
||||||| merged common ancestors
        if ps.is_peek_value_start():
            ps.skip_indent()
            value = self.get_value(ps)
            return ast.Variant(key, value, default_index)
=======
        value = self.maybe_get_pattern(ps)
        if value is None:
            raise ParseError('E0012')
>>>>>>> upstream-releases

<<<<<<< HEAD
||||||| merged common ancestors
        raise ParseError('E0012')
=======
        return ast.Variant(key, value, default_index)
>>>>>>> upstream-releases

    def get_variants(self, ps):
        variants = []
        has_default = False

        ps.skip_blank()
        while ps.is_variant_start():
            variant = self.get_variant(ps, has_default)

            if variant.default:
                has_default = True

            variants.append(variant)
            ps.expect_line_end()
            ps.skip_blank()

        if len(variants) == 0:
            raise ParseError('E0011')

        if not has_default:
            raise ParseError('E0010')

        return variants

    def get_digits(self, ps):
        num = ''

        ch = ps.take_digit()
        while ch:
            num += ch
            ch = ps.take_digit()

        if len(num) == 0:
            raise ParseError('E0004', '0-9')

        return num

    @with_span
    def get_number(self, ps):
        num = ''

        if ps.current_char == '-':
            num += '-'
            ps.next()

        num += self.get_digits(ps)

        if ps.current_char == '.':
            num += '.'
            ps.next()
            num += self.get_digits(ps)

        return ast.NumberLiteral(num)

<<<<<<< HEAD
    def maybe_get_pattern(self, ps):
        '''Parse an inline or a block Pattern, or None

        maybe_get_pattern distinguishes between patterns which start on the
        same line as the indentifier (aka inline singleline patterns and inline
        multiline patterns), and patterns which start on a new line (aka block
        patterns). The distinction is important for the dedentation logic: the
        indent of the first line of a block pattern must be taken into account
        when calculating the maximum common indent.
        '''
        ps.peek_blank_inline()
        if ps.is_value_start():
            ps.skip_to_peek()
            return self.get_pattern(ps, is_block=False)

        ps.peek_blank_block()
        if ps.is_value_continuation():
            ps.skip_to_peek()
            return self.get_pattern(ps, is_block=True)

        return None

    def maybe_get_variant_list(self, ps):
        '''Parse a VariantList, or None

        Deprecated in Syntax 0.8. VariantLists are only allowed as values of
        Terms. Values of Messages, Attributes and Variants must be Patterns.
        This method is only used in get_term.
        '''
        ps.peek_blank()
        if ps.current_peek == '{':
            start = ps.peek_offset
            ps.peek()
            ps.peek_blank_inline()
            if ps.current_peek == EOL:
                ps.peek_blank()
                if ps.is_variant_start():
                    ps.reset_peek(start)
                    ps.skip_to_peek()
                    return self.get_variant_list(ps)
||||||| merged common ancestors
    @with_span
    def get_value(self, ps):
        if ps.current_is('{'):
            ps.peek()
            ps.peek_inline_ws()
            if ps.is_peek_next_line_variant_start():
                return self.get_variant_list(ps)
=======
    def maybe_get_pattern(self, ps):
        '''Parse an inline or a block Pattern, or None

        maybe_get_pattern distinguishes between patterns which start on the
        same line as the indentifier (aka inline singleline patterns and inline
        multiline patterns), and patterns which start on a new line (aka block
        patterns). The distinction is important for the dedentation logic: the
        indent of the first line of a block pattern must be taken into account
        when calculating the maximum common indent.
        '''
        ps.peek_blank_inline()
        if ps.is_value_start():
            ps.skip_to_peek()
            return self.get_pattern(ps, is_block=False)
>>>>>>> upstream-releases

<<<<<<< HEAD
        ps.reset_peek()
        return None
||||||| merged common ancestors
        return self.get_pattern(ps)
=======
        ps.peek_blank_block()
        if ps.is_value_continuation():
            ps.skip_to_peek()
            return self.get_pattern(ps, is_block=True)
>>>>>>> upstream-releases

<<<<<<< HEAD
    @with_span
    def get_variant_list(self, ps):
        ps.expect_char('{')
        variants = self.get_variants(ps)
        ps.expect_char('}')
        return ast.VariantList(variants)
||||||| merged common ancestors
    @with_span
    def get_variant_list(self, ps):
        ps.expect_char('{')
        ps.skip_inline_ws()
        variants = self.get_variants(ps)
        ps.expect_indent()
        ps.expect_char('}')
        return ast.VariantList(variants)
=======
        return None
>>>>>>> upstream-releases

    @with_span
    def get_pattern(self, ps, is_block):
        elements = []
<<<<<<< HEAD
        if is_block:
            # A block pattern is a pattern which starts on a new line. Measure
            # the indent of this first line for the dedentation logic.
            blank_start = ps.index
            first_indent = ps.skip_blank_inline()
            elements.append(self.Indent(first_indent, blank_start, ps.index))
            common_indent_length = len(first_indent)
        else:
            common_indent_length = float('infinity')


        while ps.current_char:
            if ps.current_char == EOL:
                blank_start = ps.index
                blank_lines = ps.peek_blank_block()
                if ps.is_value_continuation():
                    ps.skip_to_peek()
                    indent = ps.skip_blank_inline()
                    common_indent_length = min(common_indent_length, len(indent))
                    elements.append(self.Indent(blank_lines + indent, blank_start, ps.index))
                    continue

                # The end condition for get_pattern's while loop is a newline
                # which is not followed by a valid pattern continuation.
                ps.reset_peek()
||||||| merged common ancestors
        ps.skip_inline_ws()

        while ps.current():
            ch = ps.current()

            # The end condition for get_pattern's while loop is a newline
            # which is not followed by a valid pattern continuation.
            if ch == '\n' and not ps.is_peek_next_line_value():
=======
        if is_block:
            # A block pattern is a pattern which starts on a new line. Measure
            # the indent of this first line for the dedentation logic.
            blank_start = ps.index
            first_indent = ps.skip_blank_inline()
            elements.append(self.Indent(first_indent, blank_start, ps.index))
            common_indent_length = len(first_indent)
        else:
            common_indent_length = float('infinity')

        while ps.current_char:
            if ps.current_char == EOL:
                blank_start = ps.index
                blank_lines = ps.peek_blank_block()
                if ps.is_value_continuation():
                    ps.skip_to_peek()
                    indent = ps.skip_blank_inline()
                    common_indent_length = min(common_indent_length, len(indent))
                    elements.append(self.Indent(blank_lines + indent, blank_start, ps.index))
                    continue

                # The end condition for get_pattern's while loop is a newline
                # which is not followed by a valid pattern continuation.
                ps.reset_peek()
>>>>>>> upstream-releases
                break

            if ps.current_char == '}':
                raise ParseError('E0027')

            if ps.current_char == '{':
                element = self.get_placeable(ps)
            else:
                element = self.get_text_element(ps)

            elements.append(element)

        dedented = self.dedent(elements, common_indent_length)
        return ast.Pattern(dedented)

    class Indent(ast.SyntaxNode):
        def __init__(self, value, start, end):
            super(FluentParser.Indent, self).__init__()
            self.value = value
            self.add_span(start, end)

    def dedent(self, elements, common_indent):
        '''Dedent a list of elements by removing the maximum common indent from
        the beginning of text lines. The common indent is calculated in
        get_pattern.
        '''
        trimmed = []

        for element in elements:
            if isinstance(element, ast.Placeable):
                trimmed.append(element)
                continue

            if isinstance(element, self.Indent):
                # Strip the common indent.
                element.value = element.value[:len(element.value) - common_indent]
                if len(element.value) == 0:
                    continue

            prev = trimmed[-1] if len(trimmed) > 0 else None
            if isinstance(prev, ast.TextElement):
                # Join adjacent TextElements by replacing them with their sum.
                sum = ast.TextElement(prev.value + element.value)
                if self.with_spans:
                    sum.add_span(prev.span.start, element.span.end)
                trimmed[-1] = sum
                continue

            if isinstance(element, self.Indent):
                # If the indent hasn't been merged into a preceding
                # TextElements, convert it into a new TextElement.
                text_element = ast.TextElement(element.value)
                if self.with_spans:
                    text_element.add_span(element.span.start, element.span.end)
                element = text_element

            trimmed.append(element)

        # Trim trailing whitespace from the Pattern.
        last_element = trimmed[-1] if len(trimmed) > 0 else None
        if isinstance(last_element, ast.TextElement):
            last_element.value = last_element.value.rstrip(' \t\n\r')
            if last_element.value == "":
                trimmed.pop()

        return trimmed

    @with_span
    def get_text_element(self, ps):
        buf = ''

        while ps.current_char:
            ch = ps.current_char

            if ch == '{' or ch == '}':
                return ast.TextElement(buf)

            if ch == EOL:
                return ast.TextElement(buf)

            buf += ch
            ps.next()

        return ast.TextElement(buf)

    def get_escape_sequence(self, ps):
        next = ps.current_char

        if next == '\\' or next == '"':
            ps.next()
            return '\\{}'.format(next), next

        if next == 'u':
<<<<<<< HEAD
            return self.get_unicode_escape_sequence(ps, next, 4)

        if next == 'U':
            return self.get_unicode_escape_sequence(ps, next, 6)
||||||| merged common ancestors
            sequence = ''
            ps.next()

            for _ in range(4):
                ch = ps.take_hex_digit()
                if ch is None:
                    raise ParseError('E0026', sequence + ps.current())
                sequence += ch

            return '\\u{}'.format(sequence)
=======
            return self.get_unicode_escape_sequence(ps, next, 4)

        if next == 'U':
            return self.get_unicode_escape_sequence(ps, next, 6)
>>>>>>> upstream-releases

        raise ParseError('E0025', next)

<<<<<<< HEAD
    def get_unicode_escape_sequence(self, ps, u, digits):
        ps.expect_char(u)
        sequence = ''
        for _ in range(digits):
            ch = ps.take_hex_digit()
            if not ch:
                raise ParseError('E0026', '\\{}{}{}'.format(u, sequence, ps.current_char))
            sequence += ch

        codepoint = int(sequence, 16)
        if codepoint <= 0xD7FF or 0xE000 <= codepoint:
            # It's a Unicode scalar value. The escape sequence is 4 or 6 digits
            # long. Convert it to a 8-digit-long \UHHHHHHHH sequence and encode
            # it as bytes, because in Python 3 decode is not available on str.
            byte_sequence = "\\U{:08x}".format(codepoint).encode('utf-8')
            unescaped = byte_sequence.decode('unicode-escape')
        else:
            # Escape sequences reresenting surrogate code points are
            # well-formed but invalid in Fluent. Replace them with U+FFFD
            # REPLACEMENT CHARACTER.
            unescaped = '\uFFFD'

        return '\\{}{}'.format(u, sequence), unescaped

||||||| merged common ancestors
=======
    def get_unicode_escape_sequence(self, ps, u, digits):
        ps.expect_char(u)
        sequence = ''
        for _ in range(digits):
            ch = ps.take_hex_digit()
            if not ch:
                raise ParseError('E0026', '\\{}{}{}'.format(u, sequence, ps.current_char))
            sequence += ch

        return '\\{}{}'.format(u, sequence)

>>>>>>> upstream-releases
    @with_span
    def get_placeable(self, ps):
        ps.expect_char('{')
        ps.skip_blank()
        expression = self.get_expression(ps)
        ps.expect_char('}')
        return ast.Placeable(expression)

    @with_span
    def get_expression(self, ps):
<<<<<<< HEAD
        selector = self.get_inline_expression(ps)

        ps.skip_blank()
||||||| merged common ancestors
        ps.skip_inline_ws()

        selector = self.get_selector_expression(ps)
=======
        selector = self.get_inline_expression(ps)
>>>>>>> upstream-releases

<<<<<<< HEAD
        if ps.current_char == '-':
            if ps.peek() != '>':
||||||| merged common ancestors
        ps.skip_inline_ws()

        if ps.current_is('-'):
            ps.peek()

            if not ps.current_peek_is('>'):
=======
        ps.skip_blank()

        if ps.current_char == '-':
            if ps.peek() != '>':
>>>>>>> upstream-releases
                ps.reset_peek()
                return selector

            if isinstance(selector, ast.MessageReference):
<<<<<<< HEAD
                raise ParseError('E0016')

            if isinstance(selector, ast.AttributeExpression) \
                   and isinstance(selector.ref, ast.MessageReference):
                raise ParseError('E0018')
||||||| merged common ancestors
                raise ParseError('E0016')

            if isinstance(selector, ast.AttributeExpression) \
               and isinstance(selector.ref, ast.MessageReference):
                raise ParseError('E0018')
=======
                if selector.attribute is None:
                    raise ParseError('E0016')
                else:
                    raise ParseError('E0018')
>>>>>>> upstream-releases

<<<<<<< HEAD
            if isinstance(selector, ast.TermReference) \
                    or isinstance(selector, ast.VariantExpression):
                raise ParseError('E0017')

            if isinstance(selector, ast.CallExpression) \
                   and isinstance(selector.callee, ast.TermReference):
||||||| merged common ancestors
            if isinstance(selector, ast.VariantExpression):
=======
            if (
                isinstance(selector, ast.TermReference)
                and selector.attribute is None
            ):
>>>>>>> upstream-releases
                raise ParseError('E0017')

            ps.next()
            ps.next()

            ps.skip_blank_inline()
            ps.expect_line_end()

            variants = self.get_variants(ps)
<<<<<<< HEAD
            return ast.SelectExpression(selector, variants)

        if isinstance(selector, ast.AttributeExpression) \
                and isinstance(selector.ref, ast.TermReference):
            raise ParseError('E0019')

        if isinstance(selector, ast.CallExpression) \
                and isinstance(selector.callee, ast.AttributeExpression):
||||||| merged common ancestors

            if len(variants) == 0:
                raise ParseError('E0011')

            # VariantLists are only allowed in other VariantLists.
            if any(isinstance(v.value, ast.VariantList) for v in variants):
                raise ParseError('E0023')

            ps.expect_indent()

            return ast.SelectExpression(selector, variants)
        elif (
            isinstance(selector, ast.AttributeExpression)
            and isinstance(selector.ref, ast.TermReference)
        ):
=======
            return ast.SelectExpression(selector, variants)

        if (
            isinstance(selector, ast.TermReference)
            and selector.attribute is not None
        ):
>>>>>>> upstream-releases
            raise ParseError('E0019')

        return selector

    @with_span
    def get_inline_expression(self, ps):
        if ps.current_char == '{':
            return self.get_placeable(ps)

<<<<<<< HEAD
        expr = self.get_simple_expression(ps)

        if isinstance(expr, (ast.NumberLiteral, ast.StringLiteral,
                ast.VariableReference)):
            return expr

        if isinstance(expr, ast.MessageReference):
            if ps.current_char == '.':
                ps.next()
                attr = self.get_identifier(ps)
                return ast.AttributeExpression(expr, attr)

            if ps.current_char == '(':
                # It's a Function. Ensure it's all upper-case.
                if not re.match('^[A-Z][A-Z_?-]*$', expr.id.name):
                    raise ParseError('E0008')
                func = ast.FunctionReference(expr.id)
                if self.with_spans:
                    func.add_span(expr.span.start, expr.span.end)
                return ast.CallExpression(func, *self.get_call_arguments(ps))

            return expr

        if isinstance(expr, ast.TermReference):
            if (ps.current_char == '['):
                ps.next()
                key = self.get_variant_key(ps)
                ps.expect_char(']')
                return ast.VariantExpression(expr, key)
||||||| merged common ancestors
        literal = self.get_literal(ps)

        if not isinstance(literal, (ast.MessageReference, ast.TermReference)):
            return literal

        ch = ps.current()
=======
        if ps.is_number_start():
            return self.get_number(ps)
>>>>>>> upstream-releases

<<<<<<< HEAD
            if (ps.current_char == '.'):
                ps.next()
                attr = self.get_identifier(ps)
                expr = ast.AttributeExpression(expr, attr)
||||||| merged common ancestors
        if (ch == '.'):
            ps.next()
            attr = self.get_identifier(ps)
            return ast.AttributeExpression(literal, attr)
=======
        if ps.current_char == '"':
            return self.get_string(ps)
>>>>>>> upstream-releases

<<<<<<< HEAD
            if (ps.current_char == '('):
                return ast.CallExpression(expr, *self.get_call_arguments(ps))
||||||| merged common ancestors
        if (ch == '['):
            ps.next()
=======
        if ps.current_char == '$':
            ps.next()
            id = self.get_identifier(ps)
            return ast.VariableReference(id)
>>>>>>> upstream-releases

<<<<<<< HEAD
            return expr

        raise ParseError('E0028')

    @with_span
    def get_simple_expression(self, ps):
        if ps.is_number_start():
            return self.get_number(ps)
        if ps.current_char == '"':
            return self.get_string(ps)
        if ps.current_char == '$':
||||||| merged common ancestors
            if isinstance(literal, ast.MessageReference):
                raise ParseError('E0024')

            key = self.get_variant_key(ps)
            ps.expect_char(']')
            return ast.VariantExpression(literal, key)

        if (ch == '('):
=======
        if ps.current_char == '-':
>>>>>>> upstream-releases
            ps.next()
<<<<<<< HEAD
            id = self.get_identifier(ps)
            return ast.VariableReference(id)
        if ps.current_char == '-':
            ps.next()
            id = self.get_identifier(ps)
            return ast.TermReference(id)
        if ps.is_identifier_start():
            id = self.get_identifier(ps)
            return ast.MessageReference(id)
        raise ParseError('E0028')
||||||| merged common ancestors

            if not re.match('^[A-Z][A-Z_?-]*$', literal.id.name):
                raise ParseError('E0008')

            positional, named = self.get_call_args(ps)
            ps.expect_char(')')

            func = ast.Function(literal.id.name)
            if (self.with_spans):
                func.add_span(literal.span.start, literal.span.end)

            return ast.CallExpression(func, positional, named)

        return literal
=======
            id = self.get_identifier(ps)
            attribute = None
            if ps.current_char == '.':
                ps.next()
                attribute = self.get_identifier(ps)
            arguments = None
            if ps.current_char == '(':
                arguments = self.get_call_arguments(ps)
            return ast.TermReference(id, attribute, arguments)

        if ps.is_identifier_start():
            id = self.get_identifier(ps)

            if ps.current_char == '(':
                # It's a Function. Ensure it's all upper-case.
                if not re.match('^[A-Z][A-Z0-9_-]*$', id.name):
                    raise ParseError('E0008')
                args = self.get_call_arguments(ps)
                return ast.FunctionReference(id, args)

            attribute = None
            if ps.current_char == '.':
                ps.next()
                attribute = self.get_identifier(ps)

            return ast.MessageReference(id, attribute)

        raise ParseError('E0028')
>>>>>>> upstream-releases

    @with_span
    def get_call_argument(self, ps):
        exp = self.get_inline_expression(ps)

        ps.skip_blank()

        if ps.current_char != ':':
            return exp

<<<<<<< HEAD
        if not isinstance(exp, ast.MessageReference):
            raise ParseError('E0009')

        ps.next()
        ps.skip_blank()
||||||| merged common ancestors
        if not isinstance(exp, ast.MessageReference):
            raise ParseError('E0009')

        ps.next()
        ps.skip_inline_ws()

        val = self.get_arg_val(ps)
=======
        if isinstance(exp, ast.MessageReference) and exp.attribute is None:
            ps.next()
            ps.skip_blank()

            value = self.get_literal(ps)
            return ast.NamedArgument(exp.id, value)
>>>>>>> upstream-releases

<<<<<<< HEAD
        value = self.get_literal(ps)
        return ast.NamedArgument(exp.id, value)
||||||| merged common ancestors
        return ast.NamedArgument(exp.id, val)
=======
        raise ParseError('E0009')
>>>>>>> upstream-releases

<<<<<<< HEAD
    def get_call_arguments(self, ps):
||||||| merged common ancestors
    def get_call_args(self, ps):
=======
    @with_span
    def get_call_arguments(self, ps):
>>>>>>> upstream-releases
        positional = []
        named = []
        argument_names = set()

        ps.expect_char('(')
        ps.skip_blank()

        while True:
            if ps.current_char == ')':
                break

            arg = self.get_call_argument(ps)
            if isinstance(arg, ast.NamedArgument):
                if arg.name.name in argument_names:
                    raise ParseError('E0022')
                named.append(arg)
                argument_names.add(arg.name.name)
            elif len(argument_names) > 0:
                raise ParseError('E0021')
            else:
                positional.append(arg)

            ps.skip_blank()

            if ps.current_char == ',':
                ps.next()
                ps.skip_blank()
                continue

            break

<<<<<<< HEAD
        ps.expect_char(')')
        return positional, named
||||||| merged common ancestors
    def get_arg_val(self, ps):
        if ps.is_number_start():
            return self.get_number(ps)
        elif ps.current_is('"'):
            return self.get_string(ps)
        raise ParseError('E0012')
=======
        ps.expect_char(')')
        return ast.CallArguments(positional, named)
>>>>>>> upstream-releases

    @with_span
    def get_string(self, ps):
<<<<<<< HEAD
        raw = ''
        value = ''
||||||| merged common ancestors
        val = ''
=======
        value = ''
>>>>>>> upstream-releases

        ps.expect_char('"')

        while True:
            ch = ps.take_char(lambda x: x != '"' and x != EOL)
            if not ch:
                break
            if ch == '\\':
<<<<<<< HEAD
                sequence, unescaped = self.get_escape_sequence(ps)
                raw += sequence
                value += unescaped
||||||| merged common ancestors
                val += self.get_escape_sequence(ps, ('{', '\\', '"'))
=======
                value += self.get_escape_sequence(ps)
>>>>>>> upstream-releases
            else:
<<<<<<< HEAD
                raw += ch
                value += ch
||||||| merged common ancestors
                val += ch
            ch = ps.take_char(lambda x: x != '"' and x != '\n')
=======
                value += ch
>>>>>>> upstream-releases

        if ps.current_char == EOL:
            raise ParseError('E0020')

        ps.expect_char('"')

<<<<<<< HEAD
        return ast.StringLiteral(raw, value)
||||||| merged common ancestors
        return ast.StringLiteral(val)
=======
        return ast.StringLiteral(value)
>>>>>>> upstream-releases

    @with_span
    def get_literal(self, ps):
        if ps.is_number_start():
            return self.get_number(ps)
        if ps.current_char == '"':
            return self.get_string(ps)
        raise ParseError('E0014')
