package rtf;

import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;

public class TextWidgetConverter implements TextConverter {
    private List<String> elements = new ArrayList<>();
    private EnumSet<FontStyle> activeStyles = EnumSet.noneOf(FontStyle.class);

    @Override
    public void ConvertCharacter(char c) {
        // Create a TextWidget for each character with current font styles
        String stylesStr = activeStyles.isEmpty() ? "normal" : 
            activeStyles.stream()
                .map(Enum::name)
                .reduce((s1, s2) -> s1 + "+" + s2)
                .orElse("normal");

        elements.add(String.format("TextWidget{text='%s', font='%s'}", 
            c, stylesStr.toLowerCase()));
    }

    @Override
    public void ConvertFontChange(FontStyle font) {
        // Toggle the font style for subsequent characters
        if (activeStyles.contains(font)) {
            activeStyles.remove(font);
        } else {
            activeStyles.add(font);
        }
    }

    @Override
    public void ConvertParagraph() {
        // Add paragraph separator
        elements.add("Paragraph");
    }

    public String GetTextWidget() {
        return String.join("\n", elements);
    }
}
