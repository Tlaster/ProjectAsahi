using ScriptReader.Model;
using System.Collections.Generic;

namespace ScriptReader
{
    public sealed class Reader
    {
        public string FileString { get; set; }

        public Reader(string fileString)
        {
            FileString = fileString;
        }

        public IList<IToken> GetBlock() => ReadLines(FileString);

        private IList<IToken> ReadLines(string str)
        {
            var block = new List<IToken>();
            var lexer = new Lexer();
            var parser = new Parser();
            var tokenList = lexer.ReadLine(str, 0);
            foreach (var item in tokenList)
            {
                do
                {
                    parser.Push(item);
                } while (parser.IsLoopingForReduce);
                if (!parser.IsAccepted) continue;
                block.Add(parser.Block);
                parser.Reset();
                if (block[block.Count - 1].Type == TokenType.Setting)
                /// ACC will not push current item,
                /// it will cause error
                {
                    parser.Push(item);
                }
            }
            return block;
        }
    }
}