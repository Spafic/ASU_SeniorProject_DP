package rtf;

public class ASCIIConverter implements TextConverter {
    private StringBuilder result = new StringBuilder();

    @Override
    public void ConvertCharacter(char c) {
        result.append(c);
    }

    @Override
    public void ConvertFontChange(FontStyle font) {
        // ASCII doesn't support font changes
    }

    @Override
    public void ConvertParagraph() {
        result.append("\n");
    }

    public String GetASCIIText() {
        return result.toString();
    }
}
