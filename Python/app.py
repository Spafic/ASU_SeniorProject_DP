from enum import Enum, auto
import re
from typing import List, Set


class FontStyle(Enum):
    BOLD = auto()
    ITALIC = auto()
    UNDERLINE = auto()


class TextConverter:
    def convert_character(self, c: str):
        pass

    def convert_font_change(self, font: FontStyle):
        pass

    def convert_paragraph(self):
        pass


class ASCIIConverter(TextConverter):
    def __init__(self):
        self._result = []

    def convert_character(self, c: str):
        self._result.append(c)

    def convert_font_change(self, font: FontStyle):
        # ASCII doesn't support font changes
        pass

    def convert_paragraph(self):
        self._result.append('\n')

    def GetASCIIText(self) -> str:
        return ''.join(self._result)


class TeXConverter(TextConverter):
    def __init__(self):
        self._result = []
        self._active_styles: Set[FontStyle] = set()

    def _escape_tex_special_char(self, c: str) -> str:
        special_chars = {
            '&': r'\&',
            '%': r'\%',
            '$': r'\$',
            '#': r'\#',
            '_': r'\_',
            '{': r'\{',
            '}': r'\}'
        }
        return special_chars.get(c, c)

    def _wrap_in_tex_commands(self, text: str) -> str:
        styles = self._active_styles
        if {FontStyle.BOLD, FontStyle.ITALIC} <= styles:
            return r'\textbf{\textit{' + text + '}}'
        if FontStyle.BOLD in styles:
            return r'\textbf{' + text + '}'
        if FontStyle.ITALIC in styles:
            return r'\textit{' + text + '}'
        if FontStyle.UNDERLINE in styles:
            return r'\underline{' + text + '}'
        return text

    def convert_character(self, c: str):
        converted_char = self._escape_tex_special_char(c)
        
        if self._active_styles:
            converted_char = self._wrap_in_tex_commands(converted_char)
        
        self._result.append(converted_char)

    def convert_font_change(self, font: FontStyle):
        if font in self._active_styles:
            self._active_styles.remove(font)
        else:
            self._active_styles.add(font)

    def convert_paragraph(self):
        self._result.append('\n\n')

    def GetTeXText(self) -> str:
        return ''.join(self._result)


class TextWidgetConverter(TextConverter):
    def __init__(self):
        self._elements: List[str] = []
        self._active_styles: Set[FontStyle] = set()

    def convert_character(self, c: str):
        # Create a TextWidget for each character with current font styles
        styles_str = 'normal' if not self._active_styles else '+'.join(
            style.name.lower() for style in self._active_styles
        )
        
        self._elements.append(f"TextWidget{{text='{c}', font='{styles_str}'}}")

    def convert_font_change(self, font: FontStyle):
        # Toggle the font style for subsequent characters
        if font in self._active_styles:
            self._active_styles.remove(font)
        else:
            self._active_styles.add(font)

    def convert_paragraph(self):
        # Add paragraph separator
        self._elements.append("Paragraph")

    def GetTextWidget(self) -> str:
        return '\n'.join(self._elements)


class RTFReader:
    def __init__(self, builder: TextConverter):
        self._builder = builder
        # Regex pattern to match different token types
        self._token_pattern = re.compile(r'\{(char|font|par)(?::([^}]+))?\}')

    def parse_rtf(self, rtf_input: str):
        # Find all tokens in the input
        tokens = self._token_pattern.finditer(rtf_input)
        last_index = 0

        for match in tokens:
            token_type = match.group(1)
            token_value = match.group(2)

            if token_type == 'char' and token_value and len(token_value) == 1:
                self._builder.convert_character(token_value)
            
            elif token_type == 'font':
                if token_value == 'bold':
                    self._builder.convert_font_change(FontStyle.BOLD)
                elif token_value == 'italic':
                    self._builder.convert_font_change(FontStyle.ITALIC)
                elif token_value == 'underline':
                    self._builder.convert_font_change(FontStyle.UNDERLINE)
            
            elif token_type == 'par':
                self._builder.convert_paragraph()

            last_index = match.end()

        # Handle any parsing errors or unexpected input
        if last_index < len(rtf_input):
            print(f"Warning: Unparsed input remains: {rtf_input[last_index:]}")


def main():
    # Enhanced sample input with more complex font toggling
    sample_input = (
        "{char:H}{char:e}{char:l}{char:l}{char:o} "
        "{font:bold}{char:W}{char:o}{char:r}{char:l}{char:d}{font:bold} "
        "{font:italic}{char:!}{font:italic}{par}"
        "{char:A}{font:bold}{font:italic}{char:B}{char:C}{font:bold}{font:italic}"
    )

    # Print the sample input for visualization
    print(f"\033[1;34mSample Input:\033[0m")
    print(sample_input)
    print(f"\n\033[1;34mProcessing...\033[0m\n")

    # Demonstrate ASCII Conversion
    print(f"\033[1;32mASCII Conversion:\033[0m")
    ascii_converter = ASCIIConverter()
    ascii_reader = RTFReader(ascii_converter)
    ascii_reader.parse_rtf(sample_input)
    print(ascii_converter.GetASCIIText())
    print(f"\n\033[1;34m-----------------------------\033[0m\n")

    # Demonstrate TeX Conversion
    print(f"\033[1;32mTeX Conversion:\033[0m")
    tex_converter = TeXConverter()
    tex_reader = RTFReader(tex_converter)
    tex_reader.parse_rtf(sample_input)
    print(tex_converter.GetTeXText())
    print(f"\n\033[1;34m-----------------------------\033[0m\n")

    # Demonstrate Text Widget Conversion
    print(f"\033[1;32mText Widget Conversion:\033[0m")
    widget_converter = TextWidgetConverter()
    widget_reader = RTFReader(widget_converter)
    widget_reader.parse_rtf(sample_input)
    print(widget_converter.GetTextWidget())
    print(f"\n\033[1;34m-----------------------------\033[0m\n")


if __name__ == "__main__":
    main()
