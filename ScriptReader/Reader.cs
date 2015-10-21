using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ScriptReader.Model;
using Windows.Foundation;
using Windows.Storage;

namespace ScriptReader
{
    public sealed class Reader
    {
        public string FileString { get; set; }

        public Reader(string fileString)
        {
            FileString = fileString;
        }

        public IList<Block> GetBlock() => ReadLines(FileString);

        private IList<Block> ReadLines(string str)
        {
            List<Block> block = new List<Block>();
            Lexer lexer = new Lexer();
            Parser parser = new Parser();
            var tokenList = lexer.ReadLine(str, 0);
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
                    if (block[block.Count - 1].BlockType == BlockTypes.SETTINGS)
                    /// ACC will not push current item,
                    /// it will cause error 
                    {
                        parser.Push(item);
                    }
                }
            }
            return block;
        }
    }
}
