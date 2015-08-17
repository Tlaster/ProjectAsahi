using System.Collections.Generic;

namespace FileManager.Model
{
    public sealed class SaveModel
    {
        public int SaveIndex { get; set; }
        public string CurrentFilePath { get; set; }
        public string BackgroundPath { get; set; }
        public string BGMPath { get; set; }
        public int BlockPosition { get; set; }
        public bool IsMultipleLanguage { get; set; }
        public string NextFilePath { get; set; }
        public IList<CharaModel> CharaList { get; set; }
        public string SaveTime { get; set; }
    }
}
