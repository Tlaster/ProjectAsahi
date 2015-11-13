using ScriptReader.Model;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace ScriptReader
{
    internal class Lexer
    {
        private const string RegexPat = "(?s)((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")|(BGM|Chara|Background|Face|Content)|(Path|Position_X|Position_Y|Name|Value|Method|Title|Time|Voice|Deep|NextFile|MultipleLanguage|TextLayoutBackground|ImageWidth|ImageHeight|FontSize|DefaultLanguage)|(TAB|SELECTION)|(#define|#)|(true|false)|(;)|(=)|({|})|(\\$)|[A-Z_a-z][A-Z_a-z0-9]*|\\p{P})";

        internal IEnumerable<Token> ReadLine(string str, int line)
        {
            List<Token> tokenList = new List<Token>();
            Regex regex = new Regex(RegexPat);
            var matches = regex.Matches(str);
            for (int i = 0; i < matches.Count; i++)
            {
                if (matches[i].Groups[1].Success && !matches[i].Groups[2].Success)
                {
                    if (matches[i].Groups[3].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Number));
                    }
                    else if (matches[i].Groups[4].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value.Replace("\"", ""), SyntaxType.String));
                    }
                    else if (matches[i].Groups[6].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.ElementName));
                    }
                    else if (matches[i].Groups[7].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Attribute));
                    }
                    else if (matches[i].Groups[8].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Type));
                    }
                    else if (matches[i].Groups[9].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Sharp));
                    }
                    else if (matches[i].Groups[10].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Boolean));
                    }
                    else if (matches[i].Groups[11].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Break));
                    }
                    else if (matches[i].Groups[12].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Equals));
                    }
                    else if (matches[i].Groups[13].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.Bracket));
                    }
                    else if (matches[i].Groups[14].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, SyntaxType.USD));
                    }
                    else
                    {
                        throw new TokenException($"Unexpected word input at line {line}");
                    }
                }
            }
            return tokenList;
        }
    }
}