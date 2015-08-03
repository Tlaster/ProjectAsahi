using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using ScriptReader.Model;
using Windows.Foundation;
using Windows.Storage;

namespace ScriptReader
{
    public sealed class Reader
    {
        public string FilePath { get; set; }
        public Block[] Block { get; set; }

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
            Lexer lexer = new Lexer();
            Parser parser = new Parser();
            var lines = await GetFileTextLinesAsync();
            List<Block> block = new List<Block>();
            for (int i = 0; i < lines.Count; i++)
            {
                var tokenList = lexer.ReadLine(lines[i], i);
                foreach (var item in tokenList)
                {
                    do
                    {
                        parser.Push(item);
                    } while (parser.IsLoopForReduce);
                    if (parser.IsAccepted)
                    {
                        block.Add(parser.Block);
                        parser.Reset();
                    }

                }
            }
            Block = block.ToArray();
        }

        private async Task<IList<string>> GetFileTextLinesAsync()
        {
            var path = $"ms-appx:///{FilePath}";
            var file = await StorageFile.GetFileFromApplicationUriAsync(new Uri(path));
            return await FileIO.ReadLinesAsync(file, Windows.Storage.Streams.UnicodeEncoding.Utf16LE);
        }
    }
}
