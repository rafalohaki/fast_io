#pragma once

namespace fast_io::char_category
{

template<std::integral char_type>
inline constexpr bool is_c_alnum(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':case L'G':case L'H':case L'I':case L'J':case L'K':case L'L':case L'M':case L'N':case L'O':case L'P':case L'Q':case L'R':case L'S':case L'T':case L'U':case L'V':case L'W':case L'X':case L'Y':case L'Z':case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'g':case L'h':case L'i':case L'j':case L'k':case L'l':case L'm':case L'n':case L'o':case L'p':case L'q':case L'r':case L's':case L't':case L'u':case L'v':case L'w':case L'x':case L'y':case L'z':return true;default:return false;}
	else
switch(ch){case u8'0':case u8'1':case u8'2':case u8'3':case u8'4':case u8'5':case u8'6':case u8'7':case u8'8':case u8'9':case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':case u8'G':case u8'H':case u8'I':case u8'J':case u8'K':case u8'L':case u8'M':case u8'N':case u8'O':case u8'P':case u8'Q':case u8'R':case u8'S':case u8'T':case u8'U':case u8'V':case u8'W':case u8'X':case u8'Y':case u8'Z':case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'g':case u8'h':case u8'i':case u8'j':case u8'k':case u8'l':case u8'm':case u8'n':case u8'o':case u8'p':case u8'q':case u8'r':case u8's':case u8't':case u8'u':case u8'v':case u8'w':case u8'x':case u8'y':case u8'z':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_alpha(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':case L'G':case L'H':case L'I':case L'J':case L'K':case L'L':case L'M':case L'N':case L'O':case L'P':case L'Q':case L'R':case L'S':case L'T':case L'U':case L'V':case L'W':case L'X':case L'Y':case L'Z':case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'g':case L'h':case L'i':case L'j':case L'k':case L'l':case L'm':case L'n':case L'o':case L'p':case L'q':case L'r':case L's':case L't':case L'u':case L'v':case L'w':case L'x':case L'y':case L'z':return true;default:return false;}
	else
switch(ch){case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':case u8'G':case u8'H':case u8'I':case u8'J':case u8'K':case u8'L':case u8'M':case u8'N':case u8'O':case u8'P':case u8'Q':case u8'R':case u8'S':case u8'T':case u8'U':case u8'V':case u8'W':case u8'X':case u8'Y':case u8'Z':case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'g':case u8'h':case u8'i':case u8'j':case u8'k':case u8'l':case u8'm':case u8'n':case u8'o':case u8'p':case u8'q':case u8'r':case u8's':case u8't':case u8'u':case u8'v':case u8'w':case u8'x':case u8'y':case u8'z':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_lower(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'g':case L'h':case L'i':case L'j':case L'k':case L'l':case L'm':case L'n':case L'o':case L'p':case L'q':case L'r':case L's':case L't':case L'u':case L'v':case L'w':case L'x':case L'y':case L'z':return true;default:return false;}
	else
switch(ch){case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'g':case u8'h':case u8'i':case u8'j':case u8'k':case u8'l':case u8'm':case u8'n':case u8'o':case u8'p':case u8'q':case u8'r':case u8's':case u8't':case u8'u':case u8'v':case u8'w':case u8'x':case u8'y':case u8'z':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_upper(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':case L'G':case L'H':case L'I':case L'J':case L'K':case L'L':case L'M':case L'N':case L'O':case L'P':case L'Q':case L'R':case L'S':case L'T':case L'U':case L'V':case L'W':case L'X':case L'Y':case L'Z':return true;default:return false;}
	else
switch(ch){case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':case u8'G':case u8'H':case u8'I':case u8'J':case u8'K':case u8'L':case u8'M':case u8'N':case u8'O':case u8'P':case u8'Q':case u8'R':case u8'S':case u8'T':case u8'U':case u8'V':case u8'W':case u8'X':case u8'Y':case u8'Z':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_digit(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':return true;default:return false;}
	else
switch(ch){case u8'0':case u8'1':case u8'2':case u8'3':case u8'4':case u8'5':case u8'6':case u8'7':case u8'8':case u8'9':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_xdigit(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':return true;default:return false;}
	else
switch(ch){case u8'0':case u8'1':case u8'2':case u8'3':case u8'4':case u8'5':case u8'6':case u8'7':case u8'8':case u8'9':case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_punct(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '!':case '\"':case '#':case '$':case '%':case '&':case '\'':case '(':case ')':case '*':case '+':case ',':case '-':case '.':case '/':case ':':case ';':case '<':case '=':case '>':case '?':case '@':case '[':case '\\':case ']':case '^':case '_':case '`':case '{':case '|':case '}':case '~':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'!':case L'\"':case L'#':case L'$':case L'%':case L'&':case L'\'':case L'(':case L')':case L'*':case L'+':case L',':case L'-':case L'.':case L'/':case L':':case L';':case L'<':case L'=':case L'>':case L'?':case L'@':case L'[':case L'\\':case L']':case L'^':case L'_':case L'`':case L'{':case L'|':case L'}':case L'~':return true;default:return false;}
	else
switch(ch){case u8'!':case u8'\"':case u8'#':case u8'$':case u8'%':case u8'&':case u8'\'':case u8'(':case u8')':case u8'*':case u8'+':case u8',':case u8'-':case u8'.':case u8'/':case u8':':case u8';':case u8'<':case u8'=':case u8'>':case u8'?':case u8'@':case u8'[':case u8'\\':case u8']':case u8'^':case u8'_':case u8'`':case u8'{':case u8'|':case u8'}':case u8'~':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_graph(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '!':case '\"':case '#':case '$':case '%':case '&':case '\'':case '(':case ')':case '*':case '+':case ',':case '-':case '.':case '/':case ':':case ';':case '<':case '=':case '>':case '?':case '@':case '[':case '\\':case ']':case '^':case '_':case '`':case '{':case '|':case '}':case '~':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':case L'G':case L'H':case L'I':case L'J':case L'K':case L'L':case L'M':case L'N':case L'O':case L'P':case L'Q':case L'R':case L'S':case L'T':case L'U':case L'V':case L'W':case L'X':case L'Y':case L'Z':case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'g':case L'h':case L'i':case L'j':case L'k':case L'l':case L'm':case L'n':case L'o':case L'p':case L'q':case L'r':case L's':case L't':case L'u':case L'v':case L'w':case L'x':case L'y':case L'z':case L'!':case L'\"':case L'#':case L'$':case L'%':case L'&':case L'\'':case L'(':case L')':case L'*':case L'+':case L',':case L'-':case L'.':case L'/':case L':':case L';':case L'<':case L'=':case L'>':case L'?':case L'@':case L'[':case L'\\':case L']':case L'^':case L'_':case L'`':case L'{':case L'|':case L'}':case L'~':return true;default:return false;}
	else
switch(ch){case u8'0':case u8'1':case u8'2':case u8'3':case u8'4':case u8'5':case u8'6':case u8'7':case u8'8':case u8'9':case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':case u8'G':case u8'H':case u8'I':case u8'J':case u8'K':case u8'L':case u8'M':case u8'N':case u8'O':case u8'P':case u8'Q':case u8'R':case u8'S':case u8'T':case u8'U':case u8'V':case u8'W':case u8'X':case u8'Y':case u8'Z':case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'g':case u8'h':case u8'i':case u8'j':case u8'k':case u8'l':case u8'm':case u8'n':case u8'o':case u8'p':case u8'q':case u8'r':case u8's':case u8't':case u8'u':case u8'v':case u8'w':case u8'x':case u8'y':case u8'z':case u8'!':case u8'\"':case u8'#':case u8'$':case u8'%':case u8'&':case u8'\'':case u8'(':case u8')':case u8'*':case u8'+':case u8',':case u8'-':case u8'.':case u8'/':case u8':':case u8';':case u8'<':case u8'=':case u8'>':case u8'?':case u8'@':case u8'[':case u8'\\':case u8']':case u8'^':case u8'_':case u8'`':case u8'{':case u8'|':case u8'}':case u8'~':return true;default:return false;}
}

template<std::integral char_type>
inline constexpr bool is_c_print(char_type ch) noexcept
{
	if constexpr(std::same_as<char,char_type>)
switch(ch){case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '!':case '\"':case '#':case '$':case '%':case '&':case '\'':case '(':case ')':case '*':case '+':case ',':case '-':case '.':case '/':case ':':case ';':case '<':case '=':case '>':case '?':case '@':case '[':case '\\':case ']':case '^':case '_':case '`':case '{':case '|':case '}':case '~':case ' ':return true;default:return false;}
	else if constexpr(std::same_as<wchar_t,char_type>)
switch(ch){case L'0':case L'1':case L'2':case L'3':case L'4':case L'5':case L'6':case L'7':case L'8':case L'9':case L'A':case L'B':case L'C':case L'D':case L'E':case L'F':case L'G':case L'H':case L'I':case L'J':case L'K':case L'L':case L'M':case L'N':case L'O':case L'P':case L'Q':case L'R':case L'S':case L'T':case L'U':case L'V':case L'W':case L'X':case L'Y':case L'Z':case L'a':case L'b':case L'c':case L'd':case L'e':case L'f':case L'g':case L'h':case L'i':case L'j':case L'k':case L'l':case L'm':case L'n':case L'o':case L'p':case L'q':case L'r':case L's':case L't':case L'u':case L'v':case L'w':case L'x':case L'y':case L'z':case L'!':case L'\"':case L'#':case L'$':case L'%':case L'&':case L'\'':case L'(':case L')':case L'*':case L'+':case L',':case L'-':case L'.':case L'/':case L':':case L';':case L'<':case L'=':case L'>':case L'?':case L'@':case L'[':case L'\\':case L']':case L'^':case L'_':case L'`':case L'{':case L'|':case L'}':case L'~':case L' ':return true;default:return false;}
	else
switch(ch){case u8'0':case u8'1':case u8'2':case u8'3':case u8'4':case u8'5':case u8'6':case u8'7':case u8'8':case u8'9':case u8'A':case u8'B':case u8'C':case u8'D':case u8'E':case u8'F':case u8'G':case u8'H':case u8'I':case u8'J':case u8'K':case u8'L':case u8'M':case u8'N':case u8'O':case u8'P':case u8'Q':case u8'R':case u8'S':case u8'T':case u8'U':case u8'V':case u8'W':case u8'X':case u8'Y':case u8'Z':case u8'a':case u8'b':case u8'c':case u8'd':case u8'e':case u8'f':case u8'g':case u8'h':case u8'i':case u8'j':case u8'k':case u8'l':case u8'm':case u8'n':case u8'o':case u8'p':case u8'q':case u8'r':case u8's':case u8't':case u8'u':case u8'v':case u8'w':case u8'x':case u8'y':case u8'z':case u8'!':case u8'\"':case u8'#':case u8'$':case u8'%':case u8'&':case u8'\'':case u8'(':case u8')':case u8'*':case u8'+':case u8',':case u8'-':case u8'.':case u8'/':case u8':':case u8';':case u8'<':case u8'=':case u8'>':case u8'?':case u8'@':case u8'[':case u8'\\':case u8']':case u8'^':case u8'_':case u8'`':case u8'{':case u8'|':case u8'}':case u8'~':case u8' ':return true;default:return false;}
}

/*
All Ascii based charset, only 6 character is supported
space (0x20, ' ')
form feed (0x0c, '\f')
line feed (0x0a, '\n')
carriage return (0x0d, '\r')
horizontal tab (0x09, '\t')
vertical tab (0x0b, '\v')

Any other exec-charset, besides these 6 characters, they can optionally support others, but too many are not allowed.

For EBCDIC NL should also get supported.
ASCII: space (0x20, ' '), EBCDIC:32
ASCII: form feed (0x0c, '\f'), EBCDIC:12
ASCII: line feed (0x0a, '\n'), EBCDIC:10
ASCII: carriage return (0x0d, '\r'), EBCDIC:13
ASCII: horizontal tab (0x09, '\t'), EBCDIC:9
ASCII: vertical tab (0x0b, '\v'), EBCDIC:11
EBCDIC specific: NL:21
*/

template<std::integral char_type>
inline constexpr bool is_c_space(char_type ch) noexcept
{
	if constexpr(fast_io::details::exec_charset_is_ebcdic<char_type>())
	{
		switch(ch)
		{
			case 5:case 11:case 12:case 13:
			case 21:case 37:case 64:return true;
			default:
			return false;
		};
	}
	else
	{
		switch(ch)
		{
			case 9:case 10:case 11:
			case 12:case 13:case 32:return true;
			default:return false;
		};
	}
}

}