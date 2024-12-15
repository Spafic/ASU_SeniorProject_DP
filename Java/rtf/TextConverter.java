package rtf;

public interface TextConverter {
    void ConvertCharacter(char c);
    void ConvertFontChange(FontStyle font);
    void ConvertParagraph();
}
