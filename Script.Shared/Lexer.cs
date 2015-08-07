using ScriptReader.Model;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace ScriptReader
{
    internal class Lexer
    {
        private const string regexPat = "(?s)((//.*)|([0-9]+)|(\"(\\\\\"|\\\\\\\\|\\\\n|[^\"])*\")|(BGM|Chara|Background|Face|Content)|(Path|Position_X|Position_Y|Name|Value|Method|Title|Time|Voice|NextFile|MultipleLanguage)|(TAB|SELECTION)|(#)|(true|false)|(;)|(=)|({|})|(\\$)|[A-Z_a-z][A-Z_a-z0-9]*|\\p{P})";

        internal IEnumerable<Token> ReadLine(string str, int line)
        {
            List<Token> tokenList = new List<Token>();
            Regex regex = new Regex(regexPat);
            var matches = regex.Matches(str);
            for (int i = 0; i < matches.Count; i++)
            {
                if (matches[i].Groups[1].Success && !matches[i].Groups[2].Success)
                {
                    if (matches[i].Groups[3].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Number));
                    }
                    else if (matches[i].Groups[4].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value.Replace("\"", ""), TokenType.String));
                    }
                    else if (matches[i].Groups[6].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.ElementName));
                    }
                    else if (matches[i].Groups[7].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Attribute));
                    }
                    else if (matches[i].Groups[8].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Type));
                    }
                    else if (matches[i].Groups[9].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Sharp));
                    }
                    else if (matches[i].Groups[10].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Boolean));
                    }
                    else if (matches[i].Groups[11].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Break));
                    }
                    else if (matches[i].Groups[12].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Equals));
                    }
                    else if (matches[i].Groups[13].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.Bracket));
                    }
                    else if(matches[i].Groups[14].Success)
                    {
                        tokenList.Add(new Token(line, matches[i].Value, TokenType.USD));
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
