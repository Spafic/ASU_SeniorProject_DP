package rtf;

import java.util.EnumSet;

public class TeXConverter implements TextConverter {
    private StringBuilder result = new StringBuilder();
    private EnumSet<FontStyle> activeStyles = EnumSet.noneOf(FontStyle.class);

    @Override
    public void ConvertCharacter(char c) {
        String convertedChar = escapeTeXSpecialChar(c);

        // Apply active styles
        if (!activeStyles.isEmpty()) {
            convertedChar = wrapInTeXCommands(convertedChar);
        }

        result.append(convertedChar);
    }

    @Override
    public void ConvertFontChange(FontStyle font) {
        if (activeStyles.contains(font)) {
            activeStyles.remove(font);
        } else {
            activeStyles.add(font);
        }
    }

    @Override
    public void ConvertParagraph() {
        result.append("\n\n");
    }

    private String escapeTeXSpecialChar(char c) {
        switch (c) {
            case '&': return "\\&";
            case '%': return "\\%";
            case '$': return "\\$";
            case '#': return "\\#";
            case '_': return "\\_";
            case '{': return "\\{";
            case '}': return "\\}";
            default: return String.valueOf(c);
        }
    }

    private String wrapInTeXCommands(String text) {
        if (activeStyles.containsAll(EnumSet.of(FontStyle.BOLD, FontStyle.ITALIC))) {
            return "\\textbf{\\textit{" + text + "}}";
        }
        if (activeStyles.contains(FontStyle.BOLD)) {
            return "\\textbf{" + text + "}";
        }
        if (activeStyles.contains(FontStyle.ITALIC)) {
            return "\\textit{" + text + "}";
        }
        if (activeStyles.contains(FontStyle.UNDERLINE)) {
            return "\\underline{" + text + "}";
        }
        return text;
    }

    public String GetTeXText() {
        return result.toString();
    }
}
