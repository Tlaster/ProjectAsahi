namespace ScriptReader.Model
{

    public sealed class Attribute
    {
        public int Line { get; }
        public string AttributeValue { get; }
        public AttributeTypes AttributeType { get; }
        public AttributeValueType ValueType { get; }

        public Attribute(int line, AttributeTypes atttype, string attributeValue, AttributeValueType type)
        {
            Line = line;
            AttributeType = atttype;
            AttributeValue = attributeValue;
            ValueType = type;
        }

        internal Attribute(AttributeToken item)
        {
            Line = item.Line;
            AttributeValue = item.Value;
            ValueType = item.ValueType;
            AttributeType = item.AttributeType;
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
        internal Block(BlockToken item)
        {
            BlockType = item.BlockType;
            ElementList = new Element(item.ElementList);
        }

        public Block Next { get; internal set; }

    }


}
