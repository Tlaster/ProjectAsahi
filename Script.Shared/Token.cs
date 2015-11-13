using System;

namespace ScriptReader.Model
{
    public interface IToken
    {
        int Line { get; }
        string Value { get; }
        TokenType Type { get; }
    }

    internal class Token : IToken
    {
        public TokenType Type { get; } = TokenType.Token;
        public string Value { get; }
        public int Line { get; }
        internal SyntaxType ValueType { get; }

        public Token(int line, string value, SyntaxType type)
        {
            Line = line;
            Value = value;
            ValueType = type;
        }
    }

    public sealed class Attribute : IToken
    {
        public TokenType Type { get; } = TokenType.Attribute;
        public bool IsGlobal { get; internal set; }
        public int Line { get; }
        public string Value { get; }
        public AttributeTypes AttributeType { get; }
        public AttributeValueType ValueType { get; }

        internal Attribute(int line, AttributeTypes atttype, string value, AttributeValueType type, bool isGlobal = false)
        {
            Line = line;
            AttributeType = atttype;
            Value = value;
            ValueType = type;
            IsGlobal = isGlobal;
            AttributeValueCheck();
        }

        private void AttributeValueCheck()
        {
            var isError = false;
            switch (AttributeType)
            {
                case AttributeTypes.TextLayoutBackground:
                case AttributeTypes.NextFile:
                case AttributeTypes.Title:
                case AttributeTypes.Path:
                case AttributeTypes.Voice:
                case AttributeTypes.Name:
                case AttributeTypes.DefaultLanguage:
                case AttributeTypes.Value:
                    isError = ValueType != AttributeValueType.String;
                    break;

                case AttributeTypes.FontSize:
                case AttributeTypes.ImageHeight:
                case AttributeTypes.ImageWidth:
                case AttributeTypes.Deep:
                case AttributeTypes.Time:
                case AttributeTypes.Position_X:
                case AttributeTypes.Position_Y:
                    isError = ValueType != AttributeValueType.Number;
                    break;

                case AttributeTypes.Method:
                    isError = ValueType != AttributeValueType.String && (Value != "ADD" || Value != "REMOVE" || Value != "CHANGE");
                    break;

                case AttributeTypes.MultipleLanguage:
                    isError = ValueType != AttributeValueType.Boolean;
                    break;

                default:
                    break;
            }
            if (isError)
            {
                throw new ParseException($"{AttributeType} can not have invalid attribute value at line {Line}");
            }
        }

        public Attribute Next { get; internal set; }
    }

    public sealed class Element : IToken
    {
        public TokenType Type { get; } = TokenType.Element;
        public string Value { get; }
        public int Line { get; }
        public ElementTypes ElementType { get; }
        public Attribute AttributeList { get; set; }

        internal Element(int line, ElementTypes elementtype, Attribute list)
        {
            Line = line;
            ElementType = elementtype;
            AttributeList = list;
            AttributeCheck();
        }

        internal Element(int line, ElementTypes types, string value)
        {
            Line = line;
            ElementType = types;
            ElementCheck(value);
        }

        private void ElementCheck(string value)
        {
            var type = AttributeTypes.Path;
            switch (ElementType)
            {
                case ElementTypes.Content:
                    type = AttributeTypes.Value;
                    break;

                default:
                    break;
            }
            AttributeList = new Attribute(Line, type, value, AttributeValueType.String);
        }

        private bool BgmElementCheck()
        {
            var hasPath = false;
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    default:
                        item.Value.Replace("\\", "/");
                        break;
                }
            }
            return !hasPath;
        }

        private bool CharaElementCheck()
        {
            bool hasName = false, hasMethod = false, hasPath = false;
            var methodType = "";
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
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

                    default:
                        break;
                }
            }
            return !hasName || !hasMethod || ((methodType == "ADD" || methodType == "CHANGE") && !hasPath);
        }

        private bool BackgroundElementCheck()
        {
            var hasPath = false;
            for (var item = AttributeList; item != null; item = item.Next)
            {
                switch (item.AttributeType)
                {
                    case AttributeTypes.Path:
                        hasPath = true;
                        break;

                    default:
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
                    case AttributeTypes.Path:
                        hasPath = true;
                        break;

                    default:
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
                    case AttributeTypes.Voice:
                    case AttributeTypes.NextFile:
                        item.Value.Replace("\\", "/");
                        break;

                    case AttributeTypes.Title:
                    case AttributeTypes.Value:
                        break;

                    default:
                        return true;
                }
            }
            return false;
        }

        private void AttributeCheck()
        {
            bool isError = false;
            switch (ElementType)
            {
                case ElementTypes.BGM:
                    isError = BgmElementCheck();
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
                throw new ParseException($"{ElementType} can not have invalid attribute at line {Line}");
            }
        }

        public Element Next { get; internal set; }
    }

    public sealed class Content : IToken
    {
        public string Value { get; }
        public int Line { get; }
        public TokenType Type { get; } = TokenType.Content;

        internal Content(string value, int line = 0)
        {
            Line = line;
            Value = value;
        }
    }

    public sealed class Selection : IToken
    {
        public string Value { get; }
        public int Line { get; }
        public TokenType Type { get; } = TokenType.Selection;
        public Element ElementList { get; private set; }

        public Selection(Element elementlist)
        {
            ElementList = elementlist;
        }
    }

    public sealed class Setting : IToken
    {
        public string Value { get; }
        public int Line { get; }
        public TokenType Type { get; } = TokenType.Setting;
        public Attribute SettingList { get; private set; }

        public Setting(Attribute list)
        {
            SettingList = list;
        }
    }

    public sealed class Tab : IToken
    {
        public string Value { get; }
        public int Line { get; }
        public TokenType Type { get; } = TokenType.Tab;
        public Element ElementList { get; }

        internal Tab(Element element)
        {
            ElementList = element;
        }
    }

    internal sealed class TokenException : Exception
    {
        internal TokenException(string message) : base(message)
        {
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
        Setting,
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
        Deep,
        NextFile,
        MultipleLanguage,
        TextLayoutBackground,
        ImageWidth,
        ImageHeight,
        FontSize,
        DefaultLanguage,
    }

    public enum SyntaxType
    {
        Sharp,
        USD,
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

    public enum TokenType
    {
        Attribute,
        Element,
        Setting,
        Content,
        Selection,
        Tab,
        Token,
    }
}