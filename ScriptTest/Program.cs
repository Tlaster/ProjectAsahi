using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ScriptReader;
using ScriptReader.Model;

namespace ScriptTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Reader reader = new Reader();
            reader.ReadFile();
        }
    }

    class Reader
    {

        public Block[] Block { get; set; }
        public void ReadFile()
        {
            Lexer lexer = new Lexer();
            Parser parser = new Parser();
            var lines = GetFileTextLinesAsync().ToList();
            List<Block> block = new List<Block>();
            for (int i = 0; i < lines.Count; i++)
            {
                var tokenList = lexer.ReadLine(lines[i], i);
                foreach (var item in tokenList)
                {
                    do
                    {
                        parser.Push(item);
                    } while (parser.isLoopingForReduce);
                    if (parser.isAccepted)
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
            }
            Block = block.ToArray();
        }

        private IEnumerable<string> GetFileTextLinesAsync()
        {
            var path = $"{AppDomain.CurrentDomain.BaseDirectory}1.txt";
            var lines = File.ReadLines(path, Encoding.BigEndianUnicode);
            return lines;
        }
    }
}
