using System.Collections.Generic;
using System.Runtime.Serialization;

namespace FileManager.Model
{
    [DataContract]
    public sealed class SaveModel
    {
        [DataMember]
        public int SaveIndex { get; set; }
        [DataMember]
        public string CurrentFilePath { get; set; }
        [DataMember]
        public string BackgroundPath { get; set; }
        [DataMember]
        public string BGMPath { get; set; }
        [DataMember]
        public int BlockPosition { get; set; }
        [DataMember]
        public BoolSetting IsMultipleLanguage { get; set; }
        [DataMember]
        public StringSetting TextLayoutBackground { get; set; }
        [DataMember]
        public FloatSetting ImageHeight { get; set; }
        [DataMember]
        public FloatSetting ImageWidth { get; set; }
        [DataMember]
        public FloatSetting FontSize { get; set; }
        [DataMember]
        public string NextFilePath { get; set; }
        [DataMember]
        public IList<CharaModel> CharaList { get; set; }
        [DataMember]
        public string SaveTime { get; set; }
    }
    [DataContract]
    public sealed class StringSetting
    {
        public StringSetting(string setvalue,bool isglobal,bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        [DataMember]
        public string Value { get; set; }
        [DataMember]
        public bool IsGlobal { get; set; }
        [DataMember]
        public bool IsSetted { get; set; }
    }
    [DataContract]
    public sealed class BoolSetting
    {
        public BoolSetting(bool setvalue, bool isglobal, bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        [DataMember]
        public bool Value { get; set; }
        [DataMember]
        public bool IsGlobal { get; set; }
        [DataMember]
        public bool IsSetted { get; set; }
    }
    [DataContract]
    public sealed class FloatSetting
    {
        public FloatSetting(float setvalue, bool isglobal, bool issetted)
        {
            Value = setvalue;
            IsGlobal = isglobal;
            IsSetted = issetted;
        }
        [DataMember]
        public float Value { get; set; }
        [DataMember]
        public bool IsGlobal { get; set; }
        [DataMember]
        public bool IsSetted { get; set; }
    }
}
