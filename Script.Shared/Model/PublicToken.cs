namespace ScriptReader.Model
{

    public sealed class Attribute
    {
        public bool IsGlobal { get; }
        public int Line { get; }
        public string AttributeValue { get; }
        public AttributeTypes AttributeType { get; }
        public AttributeValueType ValueType { get; }

        public Attribute(int line, AttributeTypes atttype, string attributeValue, AttributeValueType type,bool isGlobal)
        {
            Line = line;
            AttributeType = atttype;
            AttributeValue = attributeValue;
            ValueType = type;
            IsGlobal = IsGlobal;
        }

        internal Attribute(AttributeToken item)
        {
            Line = item.Line;
            AttributeValue = item.Value;
            ValueType = item.ValueType;
            AttributeType = item.AttributeType;
            IsGlobal = item.IsGlobal;
            if (item.Next != null)
            {
                Next = new Attribute(item.Next);
            }
        }

        public Attribute Next { get; internal set; }

    }

    public sealed class Element
    {
        public ElementTypes ElementType { get; }
        public int Line { get; }
        public Attribute AttributeList { get; }
        public Element(int line, ElementTypes elementtype, Attribute list)
        {
            Line = line;
            ElementType = elementtype;
            AttributeList = list;
        }
        
        internal Element(ElementToken item)
        {
            ElementType = item.ElementType;
            Line = item.Line;
            AttributeList = new Attribute(item.AttributeList);
            if (item.Next != null)
            {
                Next = new Element(item.Next);
            }
        }


        public Element Next { get; internal set; }
    }


    public sealed class Block
    {
        public Element ElementList { get; }
        public BlockTypes BlockType { get; }
        public string Value { get; set; }
        public Attribute SettingList { get; set; }
        internal Block(BlockToken item)
        {
            BlockType = item.BlockType;
            switch (BlockType)
            {
                case BlockTypes.TAB:
                case BlockTypes.SELECTION:
                    ElementList = new Element(item.ElementList);
                    break;
                case BlockTypes.SETTINGS:
                    SettingList = new Attribute(item.SettingList);
                    break;
                case BlockTypes.CONTENT:
                    Value = item.Value;
                    break;
                default:
                    break;
            }
        }

        //public Block Next { get; internal set; }

    }


}
