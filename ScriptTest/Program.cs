using ScriptReader;
using ScriptReader.Model;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ScriptTest
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Reader reader = new Reader();
            reader.ReadFile();
        }
    }

    internal class Reader
    {
        public void ReadFile()
        {
            Lexer lexer = new Lexer();
            Parser parser = new Parser();
            var lines = GetFileTextLinesAsync().ToList();
            List<IToken> block = new List<IToken>();
            for (int i = 0; i < lines.Count; i++)
            {
                var tokenList = lexer.ReadLine(lines[i], i);
                foreach (var item in tokenList)
                {
                    do
                    {
                        parser.Push(item);
                    } while (parser.IsLoopingForReduce);
                    if (parser.IsAccepted)
                    {
                        block.Add(parser.Block);
                        parser.Reset();
                        if (block[block.Count - 1].Type == TokenType.Setting)
                        /// ACC will not push current item,
                        /// it will cause error
                        {
                            parser.Push(item);
                        }
                    }
                }
            }
        }

        private IEnumerable<string> GetFileTextLinesAsync()
        {
            var path = $"{AppDomain.CurrentDomain.BaseDirectory}1.txt";
            var lines = File.ReadLines(path, Encoding.BigEndianUnicode);
            return lines;
        }
    }
}