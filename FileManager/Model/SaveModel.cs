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
        public BoolSetting IsMultipleLanguage { get; set; }
        public StringSetting TextLayoutBackground { get; set; }
        public FloatSetting ImageHeight { get; set; }
        public FloatSetting ImageWidth { get; set; }
        public FloatSetting FontSize { get; set; }
        public string NextFilePath { get; set; }
        public IList<CharaModel> CharaList { get; set; }
        public string SaveTime { get; set; }
    }
    public sealed class StringSetting
    {
        public StringSetting(string setvalue,bool isglobal,bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        public string Value { get; set; }
        public bool IsGlobal { get; set; }
        public bool IsSetted { get; set; }
    }
    public sealed class BoolSetting
    {
        public BoolSetting(bool setvalue, bool isglobal, bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        public bool Value { get; set; }
        public bool IsGlobal { get; set; }
        public bool IsSetted { get; set; }
    }
    public sealed class FloatSetting
    {
        public FloatSetting(float setvalue, bool isglobal, bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        public float Value { get; set; }
        public bool IsGlobal { get; set; }
        public bool IsSetted { get; set; }
    }
}
