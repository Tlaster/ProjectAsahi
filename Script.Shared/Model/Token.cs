using System;

namespace ScriptReader.Model
{
    internal class Token
    {
        internal int Line { get; set; }
        internal string Text { get; set; }
        internal TokenType Type { get; set; }

        protected Token() { }

        private Token(int line, TokenType type)
        {
            Line = line;
            Type = type;
        }

        internal Token(int line, string value, TokenType type) : this(line, type)
        {
            Text = value;
        }
    }


    internal sealed class TokenException : Exception
    {
        internal TokenException() { }
        internal TokenException(string message) : base(message) { }
        internal TokenException(string message, Exception inner) : base(message, inner) { }
    }


    internal class AttributeToken : Token
    {
        internal string Value { get; }
        internal AttributeTypes AttributeType { get; }
        internal AttributeValueType ValueType { get; }

        internal AttributeToken(int line, AttributeTypes atttype, string value, AttributeValueType type)
        {
            Line = line;
            AttributeType = atttype;
            Value = value;
            ValueType = type;
        }

        private void AttributeValueCheck()
        {
            bool isError = false;
            switch (AttributeType)
            {
                case AttributeTypes.Path:
                    isError = ValueType != AttributeValueType.String;
                    break;
                case AttributeTypes.Position_X:
                    isError = ValueType != AttributeValueType.Number;
                    break;
                case AttributeTypes.Position_Y:
                    isError = ValueType != AttributeValueType.Number;
                    break;
                case AttributeTypes.Name:
                    isError = ValueType != AttributeValueType.String;
                    break;
                case AttributeTypes.Value:
                    isError = ValueType != AttributeValueType.String;
                    break;
                case AttributeTypes.Method:
                    isError = ValueType != AttributeValueType.String && (Value != "ADD" || Value != "REMOVE" || Value != "CHANGE");
                    break;
                case AttributeTypes.Voice:
                    isError = ValueType != AttributeValueType.String;
                    break;
                case AttributeTypes.Time:
                    isError = ValueType != AttributeValueType.Number;
                    break;
                case AttributeTypes.Title:
                    isError = ValueType != AttributeValueType.String;
                    break;
            }
            if (isError)
            {
                throw new ParseException($"{AttributeType.ToString()} can not have invalid attribute value at line {Line}");
            }
        }

        internal AttributeToken Next { get; set; }

    }



    internal class ElementToken : Token
    {
        internal ElementTypes ElementType { get; }
        internal AttributeToken AttributeList { get; }
        internal ElementToken(int line, ElementTypes elementtype, AttributeToken list)
        {
            Line = line;
            ElementType = elementtype;
            AttributeList = list;
            AttributeCheck();
        }

        private bool BGMElementCheck()
        {
            bool hasPath = false;
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Position_X:
                    case AttributeTypes.Position_Y:
                    case AttributeTypes.Name:
                    case AttributeTypes.Value:
                    case AttributeTypes.Method:
                    case AttributeTypes.Time:
                    case AttributeTypes.Voice:
                    case AttributeTypes.Title:
                        return true;
                    case AttributeTypes.Path:
                        item.Value.Replace("\\", "/");
                        hasPath = true;
                        break;
                }
            }
            return !hasPath;
        }

        private bool CharaElementCheck()
        {
            bool hasName = false, hasMethod = false, hasPath = false;
            string methodType = "";
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Value:
                    case AttributeTypes.Voice:
                    case AttributeTypes.Time:
                    case AttributeTypes.Title:
                        return true;
                    case AttributeTypes.Name:
                        hasName = true;
                        break;
                    case AttributeTypes.Method:
                        hasMethod = true;
                        methodType = item.Value;
                        break;
                    case AttributeTypes.Path:
                        hasPath = true;
                        break;
                }
            }
            return !hasName || !hasMethod || ((methodType == "ADD" || methodType == "CHANGE") && !hasPath);
        }

        private bool BackgroundElementCheck()
        {
            bool hasPath = false;
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Position_X:
                    case AttributeTypes.Position_Y:
                    case AttributeTypes.Name:
                    case AttributeTypes.Value:
                    case AttributeTypes.Method:
                    case AttributeTypes.Voice:
                    case AttributeTypes.Time:
                    case AttributeTypes.Title:
                        return true;
                    case AttributeTypes.Path:
                        hasPath = true;
                        break;
                }
            }
            return !hasPath;
        }

        private bool FaceElementCheck()
        {
            bool hasPath = false;
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Position_X:
                    case AttributeTypes.Position_Y:
                    case AttributeTypes.Name:
                    case AttributeTypes.Value:
                    case AttributeTypes.Method:
                    case AttributeTypes.Voice:
                    case AttributeTypes.Time:
                    case AttributeTypes.Title:
                        return true;
                    case AttributeTypes.Path:
                        hasPath = true;
                        break;
                }
            }
            return !hasPath;
        }

        private bool ContentElementCheck()
        {
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Path:
                    case AttributeTypes.Position_X:
                    case AttributeTypes.Position_Y:
                    case AttributeTypes.Name:
                    case AttributeTypes.Method:
                        return true;
                    case AttributeTypes.Voice:
                        item.Value.Replace("\\", "/");
                        break;
                }
            }
            return false;
        }

        internal void AttributeCheck()
        {
            bool isError = false;
            switch (ElementType)
            {
                case ElementTypes.BGM:
                    isError = BGMElementCheck();
                    break;
                case ElementTypes.Chara:
                    isError = CharaElementCheck();
                    break;
                case ElementTypes.Background:
                    isError = BackgroundElementCheck();
                    break;
                case ElementTypes.Face:
                    isError = FaceElementCheck();
                    break;
                case ElementTypes.Content:
                    isError = ContentElementCheck();
                    break;
            }
            if (isError)
            {
                throw new ParseException($"{ElementType.ToString()} can not have invalid attribute at line {Line}");
            }
        }

        internal ElementToken Next { get; set; }
    }

    internal class BlockToken : Token
    {
        internal ElementToken ElementList { get; }
        internal BlockTypes BlockType { get; }
        internal BlockToken(BlockTypes type, ElementToken element)
        {
            BlockType = type;
            ElementList = element;
        }
    }

    public enum AttributeValueType
    {
        String,
        Number,
        Boolean,
    }

    public enum ElementTypes
    {
        BGM,
        Chara,
        Background,
        Face,
        Content,
        SETTING,
    }

    public enum AttributeTypes
    {
        Path,
        Position_X,
        Position_Y,
        Name,
        Value,
        Method,
        Voice,
        Time,
        Title,
        NextFile,
        MultipleLanguage,
    }

    public enum TokenType
    {
        Sharp,
        Type,
        Bracket,
        ElementName,
        Attribute,
        Equals,
        String,
        Break,
        Number,
        Boolean,
    }

    public enum BlockTypes
    {
        TAB,
        SELECTION,
        SETTINGS,
    }

}