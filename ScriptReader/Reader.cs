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
        public string FilePath { get; set; }
        public IList<Block> Block { get; set; }

        public Reader(string filePath)
        {
            FilePath = filePath;
        }
        public IAsyncAction ReadFile()
        {
            return ReadFileTask().AsAsyncAction();
        }

        private async Task ReadFileTask()
        {
            var lines = await GetFileTextLinesAsync();
            var block = ReadLines(lines);
            Block = block.ToArray();
        }

        private IEnumerable<Block> ReadLines(IList<string> lines)
        {
            List<Block> block = new List<Block>();
            Lexer lexer = new Lexer();
            Parser parser = new Parser();
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
                        if (block[block.Count-1].BlockType == BlockTypes.SETTINGS)
                        /// ACC will not push current item,
                        /// it will cause error 
                        {
                            parser.Push(item);
                        }
                    }

                }
            }
            return block;
        }
        private async Task<IList<string>> GetFileTextLinesAsync()
        {
            var path = $"ms-appx:///{FilePath}";
            var file = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
            return await FileIO.ReadLinesAsync(file, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
        }
    }
}
