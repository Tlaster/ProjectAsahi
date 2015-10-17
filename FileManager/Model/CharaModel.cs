using System.Runtime.Serialization;

namespace FileManager.Model
{
    [DataContract]
    public sealed class CharaModel
    {
        [DataMember]
        public float Position_X { get; set; }
        [DataMember]
        public float Position_Y { get; set; }
        [DataMember]
        public string Name { get; set; }
        [DataMember]
        public string FilePath { get; set; }
        [DataMember]
        public int Deep { get; set; }
    }
}
