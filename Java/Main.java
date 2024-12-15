import rtf.*;

public class Main {
    public static void main(String[] args) {
        // Enhanced sample input with more complex font toggling
        String sampleInput = 
            "{char:H}{char:e}{char:l}{char:l}{char:o} " +
            "{font:bold}{char:W}{char:o}{char:r}{char:l}{char:d}{font:bold} " +
            "{font:italic}{char:!}{font:italic}{par}" +
            "{char:A}{font:bold}{font:italic}{char:B}{char:C}{font:bold}{font:italic}";

        // Print the sample input for visualization
        System.out.println("\n\033[1;34mSample Input:\033[0m");
        System.out.println(sampleInput);
        System.out.println("\n\033[1;34mProcessing...\033[0m\n");

        // Demonstrate ASCII Conversion
        System.out.println("\033[1;32mASCII Conversion:\033[0m");
        TextConverter asciiConverter = new ASCIIConverter();
        RTFReader asciiReader = new RTFReader(asciiConverter);
        asciiReader.ParseRTF(sampleInput);
        System.out.println(((ASCIIConverter) asciiConverter).GetASCIIText());
        System.out.println("\n\033[1;34m-----------------------------\033[0m\n");

        // Demonstrate TeX Conversion
        System.out.println("\033[1;32mTeX Conversion:\033[0m");
        TextConverter texConverter = new TeXConverter();
        RTFReader texReader = new RTFReader(texConverter);
        texReader.ParseRTF(sampleInput);
        System.out.println(((TeXConverter) texConverter).GetTeXText());
        System.out.println("\n\033[1;34m-----------------------------\033[0m\n");

        // Demonstrate Text Widget Conversion
        System.out.println("\033[1;32mText Widget Conversion:\033[0m");
        TextConverter widgetConverter = new TextWidgetConverter();
        RTFReader widgetReader = new RTFReader(widgetConverter);
        widgetReader.ParseRTF(sampleInput);
        System.out.println(((TextWidgetConverter) widgetConverter).GetTextWidget());
        System.out.println("\n\033[1;34m-----------------------------\033[0m\n");
    }
}
