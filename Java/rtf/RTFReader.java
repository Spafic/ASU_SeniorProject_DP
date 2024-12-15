package rtf;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RTFReader {
    private TextConverter builder;
    private static final Pattern TOKEN_PATTERN = Pattern.compile("\\{(char|font|par)(?::([^}]+))?\\}");

    public RTFReader(TextConverter builder) {
        this.builder = builder;
    }

    public void ParseRTF(String rtf_input) {
        Matcher matcher = TOKEN_PATTERN.matcher(rtf_input);
        int lastIndex = 0;

        while (matcher.find()) {
            String tokenType = matcher.group(1);
            String tokenValue = matcher.group(2);

            switch (tokenType) {
                case "char":
                    if (tokenValue != null && tokenValue.length() == 1) {
                        builder.ConvertCharacter(tokenValue.charAt(0));
                    }
                    break;
                case "font":
                    if (tokenValue != null) {
                        switch (tokenValue) {
                            case "bold":
                                builder.ConvertFontChange(FontStyle.BOLD);
                                break;
                            case "italic":
                                builder.ConvertFontChange(FontStyle.ITALIC);
                                break;
                            case "underline":
                                builder.ConvertFontChange(FontStyle.UNDERLINE);
                                break;
                        }
                    }
                    break;
                case "par":
                    builder.ConvertParagraph();
                    break;
            }

            lastIndex = matcher.end();
        }

        // Handle any parsing errors or unexpected input
        if (lastIndex < rtf_input.length()) {
            System.err.println("Warning: Unparsed input remains: " + 
                rtf_input.substring(lastIndex));
        }
    }
}
