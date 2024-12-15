#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include <sstream>
#include <memory>

// Enum for font styles
enum class FontStyle {
    BOLD,
    ITALIC,
    UNDERLINE
};

// Interface for text conversion
class TextConverter {
public:
    virtual void ConvertCharacter(char c) = 0;
    virtual void ConvertFontChange(FontStyle font) = 0;
    virtual void ConvertParagraph() = 0;
    virtual ~TextConverter() = default;
};

// Concrete implementation for ASCII conversion
class ASCIIConverter : public TextConverter {
private:
    std::ostringstream result;

public:
    void ConvertCharacter(char c) override {
        result << c;
    }

    void ConvertFontChange(FontStyle) override {
        // ASCII doesn't support font changes
    }

    void ConvertParagraph() override {
        result << "\n";
    }

    std::string GetASCIIText() {
        return result.str();
    }
};

// Concrete implementation for TeX conversion
class TeXConverter : public TextConverter {
private:
    std::ostringstream result;
    std::set<FontStyle> activeStyles;

    std::string EscapeTeXSpecialChar(char c) {
        switch (c) {
            case '&': return "\\&";
            case '%': return "\\%";
            case '$': return "\\$";
            case '#': return "\\#";
            case '_': return "\\_";
            case '{': return "\\{";
            case '}': return "\\}";
            default: return std::string(1, c);
        }
    }

    std::string WrapInTeXCommands(const std::string& text) {
        bool hasBold = activeStyles.find(FontStyle::BOLD) != activeStyles.end();
        bool hasItalic = activeStyles.find(FontStyle::ITALIC) != activeStyles.end();
        bool hasUnderline = activeStyles.find(FontStyle::UNDERLINE) != activeStyles.end();

        if (hasBold && hasItalic) {
            return "\\textbf{\\textit{" + text + "}}";
        }
        if (hasBold) {
            return "\\textbf{" + text + "}";
        }
        if (hasItalic) {
            return "\\textit{" + text + "}";
        }
        if (hasUnderline) {
            return "\\underline{" + text + "}";
        }
        return text;
    }

public:
    void ConvertCharacter(char c) override {
        std::string convertedChar = EscapeTeXSpecialChar(c);
        
        // Apply active styles
        if (!activeStyles.empty()) {
            convertedChar = WrapInTeXCommands(convertedChar);
        }
        
        result << convertedChar;
    }

    void ConvertFontChange(FontStyle font) override {
        auto it = activeStyles.find(font);
        if (it != activeStyles.end()) {
            activeStyles.erase(it);
        } else {
            activeStyles.insert(font);
        }
    }

    void ConvertParagraph() override {
        result << "\n\n";
    }

    std::string GetTeXText() {
        return result.str();
    }
};

// Concrete implementation for Text Widget conversion
class TextWidgetConverter : public TextConverter {
private:
    std::vector<std::string> elements;
    std::set<FontStyle> activeStyles;

    std::string GetStylesString() {
        if (activeStyles.empty()) {
            return "normal";
        }

        std::ostringstream stylesStr;
        bool first = true;
        for (auto style : activeStyles) {
            if (!first) {
                stylesStr << "+";
            }
            first = false;

            switch (style) {
                case FontStyle::BOLD: stylesStr << "bold"; break;
                case FontStyle::ITALIC: stylesStr << "italic"; break;
                case FontStyle::UNDERLINE: stylesStr << "underline"; break;
            }
        }
        return stylesStr.str();
    }

public:
    void ConvertCharacter(char c) override {
        std::ostringstream widgetStr;
        widgetStr << "TextWidget{text='" << c << "', font='" 
                  << GetStylesString() << "'}";
        elements.push_back(widgetStr.str());
    }

    void ConvertFontChange(FontStyle font) override {
        auto it = activeStyles.find(font);
        if (it != activeStyles.end()) {
            activeStyles.erase(it);
        } else {
            activeStyles.insert(font);
        }
    }

    void ConvertParagraph() override {
        elements.push_back("Paragraph");
    }

    std::string GetTextWidget() {
        std::ostringstream result;
        for (const auto& element : elements) {
            result << element << "\n";
        }
        return result.str();
    }
};

// RTF Reader class
class RTFReader {
private:
    std::unique_ptr<TextConverter> builder;
    std::regex tokenPattern;

public:
    RTFReader(std::unique_ptr<TextConverter> converter) 
        : builder(std::move(converter)), 
          tokenPattern(R"(\{(char|font|par)(?::([^}]+))?\})") {}

    void ParseRTF(const std::string& rtfInput) {
        std::sregex_iterator it(rtfInput.begin(), rtfInput.end(), tokenPattern);
        std::sregex_iterator end;

        size_t lastIndex = 0;

        for (; it != end; ++it) {
            std::string tokenType = (*it)[1];
            std::string tokenValue = (*it)[2];

            if (tokenType == "char" && !tokenValue.empty() && tokenValue.length() == 1) {
                builder->ConvertCharacter(tokenValue[0]);
            } else if (tokenType == "font") {
                if (tokenValue == "bold") {
                    builder->ConvertFontChange(FontStyle::BOLD);
                } else if (tokenValue == "italic") {
                    builder->ConvertFontChange(FontStyle::ITALIC);
                } else if (tokenValue == "underline") {
                    builder->ConvertFontChange(FontStyle::UNDERLINE);
                }
            } else if (tokenType == "par") {
                builder->ConvertParagraph();
            }

            lastIndex = it->position() + it->length();
        }

        // Handle any parsing errors or unexpected input
        if (lastIndex < rtfInput.length()) {
            std::cerr << "Warning: Unparsed input remains: " 
                      << rtfInput.substr(lastIndex) << std::endl;
        }
    }

    std::string GetResult() const {
        if (auto asciiConverter = dynamic_cast<ASCIIConverter*>(builder.get())) {
            return asciiConverter->GetASCIIText();
        } else if (auto texConverter = dynamic_cast<TeXConverter*>(builder.get())) {
            return texConverter->GetTeXText();
        } else if (auto widgetConverter = dynamic_cast<TextWidgetConverter*>(builder.get())) {
            return widgetConverter->GetTextWidget();
        }
        return "";
    }
};

int main() {
    // Enhanced sample input with more complex font toggling
    std::string sampleInput = 
        "{char:H}{char:e}{char:l}{char:l}{char:o} "
        "{font:bold}{char:W}{char:o}{char:r}{char:l}{char:d}{font:bold}"
        "{font:italic}{char:!}{font:italic}{par}"
        "{char:A}{font:bold}{font:italic}{char:B}{char:C}{font:bold}{font:italic}";

    // Print the sample input for visualization
    std::cout << "\033[1;34mSample Input:\033[0m" << std::endl;
    std::cout << sampleInput << std::endl;
    std::cout << "\n\033[1;34mProcessing...\033[0m\n" << std::endl;

    // Fixed ASCII Conversion
    std::cout << "\033[1;32mASCII Conversion:\033[0m" << std::endl;
    RTFReader asciiReader(std::make_unique<ASCIIConverter>());
    asciiReader.ParseRTF(sampleInput);
    std::cout << asciiReader.GetResult() << std::endl;
    std::cout << "\n\033[1;34m-----------------------------\033[0m\n" << std::endl;

    // Fixed TeX Conversion  
    std::cout << "\033[1;32mTeX Conversion:\033[0m" << std::endl;
    RTFReader texReader(std::make_unique<TeXConverter>());
    texReader.ParseRTF(sampleInput);
    std::cout << texReader.GetResult() << std::endl;
    std::cout << "\n\033[1;34m-----------------------------\033[0m\n" << std::endl;

    // Fixed Text Widget Conversion
    std::cout << "\033[1;32mText Widget Conversion:\033[0m" << std::endl;
    RTFReader widgetReader(std::make_unique<TextWidgetConverter>());
    widgetReader.ParseRTF(sampleInput);
    std::cout << widgetReader.GetResult() << std::endl;
    std::cout << "\n\033[1;34m-----------------------------\033[0m\n" << std::endl;

    return 0;
}