using System.Collections.Generic;
using ScriptReader.Model;

namespace ScriptReader
{
    internal class Parser
    {
        ///// <summary>
        ///// Grammar expression:
        ///// Block -> Type Bracket Elements Bracket
        /////        | Settings
        ///// Settings -> Setting
        /////           | Settings Setting
        ///// Setting -> Sharp Att
        ///// Elements -> Element
        /////           | Elements Element
        ///// Element -> EName Bracket Atts Bracket
        ///// Atts -> Att
        /////       | Atts Att
        ///// Att -> Name Equals Value Break
        ///// </summary>


        /// <summary>
        /// Grammar expression:
        ///1 Block -> Type Bracket Elements Bracket
        ///2        | Settings
        ///3        | Content
        ///4 Elements -> Element
        ///5           | Elements Element
        ///6 Element -> Sharp Ename Equals Value Break
        ///7          | EName Bracket Atts Bracket
        ///8 Settings -> Setting
        ///9           | Settings Setting
        ///10 Content -> USD Value Break
        ///11 Setting -> Sharp Att
        ///12 Atts -> Att
        ///13       | Atts Att
        ///14 Att -> Name Equals Value Break
        /// </summary>

        private readonly int[] _popCount = new[] { 4, 1, 1, 1, 2, 5, 4, 1, 2, 3, 2, 1, 2, 4 };

        private readonly string[][] _action = new[]
        {
            new[] {"S5","S6","S1",null,null,null,null,null,null},
            new[] {null,null,null,"S7",null,null,null,null,null},
            new[] {"S5","A2","A2",null,null,null,null,null,null},
            new[] {null,"R3","R3",null,null,null,null,null,null},
            new[] {"R8","R8","R8",null,null,null,null,null,null},
            new[] {null,null,null,null,null,"S10",null,null,null},
            new[] {null,null,null,null,null,null,null,"S11",null},
            new[] {"S14",null,null,null,"S15",null,null,null,null},
            new[] {"R9","R9","R9",null,null,null,null,null,null},
            new[] {"R11","R11","R11",null,null,null,null,null,null},
            new[] {null,null,null,null,null,null,"S16",null,null},
            new[] {null,null,null,null,null,null,null,null,"A3"},
            new[] {"S14",null,null,"A1","S15",null,null,null,null},
            new[] {"R4",null,null,"R4","R4",null,null,null,null},
            new[] {null,null,null,null,"S20",null,null,null,null},
            new[] {null,null,null,"S21",null,null,null,null,null},
            new[] {null,null,null,null,null,null,null,"S22",null},
            new[] {null,"R10","R10",null,null,null,null,null,null},
            new[] {null,"R1","R1",null,null,null,null,null,null},
            new[] {"R5",null,null,"R5","R5",null,null,null,null},
            new[] {null,null,null,null,null,null,"S23",null,null},
            new[] {null,null,null,null,null,"S10",null,null,null},
            new[] {null,null,null,null,null,null,null,null,"S26"},
            new[] {null,null,null,null,null,null,null,"S27",null},
            new[] {null,null,null,"S28",null,"S10",null,null,null},
            new[] {null,null,null,"R12",null,"R12",null,null,null},
            new[] {"R14",null,"R14","R14",null,"R14",null,null,null},
            new[] {null,null,null,null,null,null,null,null,"S30"},
            new[] {"R7",null,null,"R7","R7",null,null,null,null},
            new[] {null,null,null,"R13",null,"R13",null,null,null},
            new[] {"R6",null,null,"R6","R6",null,null,null,null},
        };

        private readonly int[][] _goto = new[]
        {
            new[] {2,4,-1,-1,-1,-1,3},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,8,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,9,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,12,13,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,19,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,24,25,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,29,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
            new[] {-1,-1,-1,-1,-1,-1,-1},
        };

        //private readonly string[][] _action = new string[][]
        //{
        //    new[] {"S4","S1",null,null,null,null,null,null},
        //    new[] {null,null,"S5",null,null,null,null,null},
        //    new[] {"S4","ACC",null,null,null,null,null,null},
        //    new[] {"R3","R3",null,null,null,null,null,null},
        //    new[] {null,null,null,null,"S8",null,null,null},
        //    new[] {null,null,null,"S11",null,null,null,null},
        //    new[] {"R4","R4",null,null,null,null,null,null},
        //    new[] {"R5","R5",null,null,null,null,null,null},
        //    new[] {null,null,null,null,null,"S12",null,null},
        //    new[] {null,null,"ACC","S11",null,null,null,null},
        //    new[] {null,null,"R6","R6",null,null,null,null},
        //    new[] {null,null,"S15",null,null,null,null,null},
        //    new[] {null,null,null,null,null,null,"S16",null},
        //    new[] {null,"R1",null,null,null,null,null,null},
        //    new[] {null,null,"R7","R7",null,null,null,null},
        //    new[] {null,null,null,null,"S8",null,null,null},
        //    new[] {null,null,null,null,null,null,null,"S19"},
        //    new[] {null,null,"S20",null,"S8",null,null,null},
        //    new[] {null,null,"R9",null,"R9",null,null,null},
        //    new[] {"R11","R11","R11",null,"R11",null,null,null},
        //    new[] {null,null,"R8","R8",null,null,null,null},
        //    new[] {null,null,"R10",null,"R10",null,null,null},
        //};

        //private readonly int[][] _goto = new int[][]
        //{
        //    new[] {2,3,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,6,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,7},
        //    new[] {-1,-1,9,10,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,14,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,17,18},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,21},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //    new[] {-1,-1,-1,-1,-1,-1},
        //};
        //private readonly int[] _popCount = new[] { 4, 1, 1, 2, 2, 1, 2, 4, 1, 2, 4 };

        internal bool isAccepted { get; set; }
        internal bool isLoopingForReduce { get; set; }

        private List<Token> _tokenStack;
        private List<int> _stateStack;
        private BlockToken _blockToken;
        private Block _block;

        internal Parser()
        {
            _tokenStack = new List<Token>();
            _stateStack = new List<int>();
            _tokenStack.Add(new Token(0, null, TokenType.Bracket));
            _stateStack.Add(0);
            isLoopingForReduce = false;
        }

        internal BlockToken BlockToken => _blockToken;
        internal Block Block => _block;

        internal void Reset()
        {
            _tokenStack = new List<Token>();
            _stateStack = new List<int>();
            _tokenStack.Add(new Token(0, null, TokenType.Bracket));
            _stateStack.Add(0);
            _blockToken = null;
            isLoopingForReduce = false;
            isAccepted = false;
        }

        internal void Push(Token token)
        {
            int wordtype = 0;
            switch (token.Type)
            {
                case TokenType.Number:
                case TokenType.Boolean:
                    wordtype = (int)TokenType.String;
                    break;
                default:
                    wordtype = (int)token.Type;
                    break;
            }

            var action = _action[_stateStack[_stateStack.Count - 1]][wordtype];
            if (action == null)
            {
                isLoopingForReduce = false;
                throw new ParseException($"Grammar error at line {token.Line}");
            }
            else if (action[0] == 'A')
            {
                isAccepted = true;
                isLoopingForReduce = false;
                BlockTypes blocktype = BlockTypes.SETTINGS;
                switch (action[1])
                {
                    case '1':///ACC for elements
                        blocktype = (BlockTypes)System.Enum.Parse(typeof(BlockTypes), _tokenStack[1].Value);
                        _blockToken = new BlockToken(blocktype, _tokenStack[_tokenStack.Count - 1] as ElementToken);
                        break;
                    case '2':///ACC for settings
                        _blockToken = new BlockToken(_tokenStack[_tokenStack.Count - 1] as AttributeToken);
                        break;
                    case '3':///ACC for content
                        _blockToken = new BlockToken(_tokenStack[_tokenStack.Count - 1].Value);
                        break;
                    default:
                        break;
                }
                _block = new Block(_blockToken);
            }
            else if (action[0] == 'S')
            {
                var statestring = "";
                for (int i = 1; i < action.Length; i++)
                {
                    statestring += action[i];
                }
                var nextstate = int.Parse(statestring);
                _stateStack.Add(nextstate);
                _tokenStack.Add(token);
                isLoopingForReduce = false;
            }
            else
            {
                isLoopingForReduce = true;
                var reducestring = "";
                for (int i = 1; i < action.Length; i++)
                {
                    reducestring += action[i];
                }
                var reducerule = int.Parse(reducestring);
                switch (reducerule)
                {
                    //case 1:/// Block -> Type Bracket Elements Bracket
                    //    break;
                    //case 2:/// Block -> Settings
                    //    break;
                    //case 3:/// Block -> Content
                    //    break;
                    case 4:/// Elements -> Element
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][2];
                        break;
                    case 5:/// Elements -> Elements Element
                        ElementToken nextele = _tokenStack[_tokenStack.Count - 1] as ElementToken;
                        ElementToken preele = _tokenStack[_tokenStack.Count - 2] as ElementToken;
                        nextele.Next = preele;
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(nextele);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][2]);
                        break;
                    case 6:/// Element -> Sharp Ename Equals Value Break
                        var eName = _tokenStack[_tokenStack.Count - 4].Value;
                        var eType = (ElementTypes)System.Enum.Parse(typeof(ElementTypes), eName);
                        ElementToken setele = new ElementToken(_tokenStack[_tokenStack.Count - 5].Line, eType, _tokenStack[_tokenStack.Count - 2].Value);
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(setele);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][3]);
                        break;
                    case 7:/// Element -> EName Bracket Atts Bracket
                        var elementName = _tokenStack[_tokenStack.Count - 4].Value;
                        var elementType = (ElementTypes)System.Enum.Parse(typeof(ElementTypes), elementName);
                        var atts = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        ElementToken element = new ElementToken(atts.Line, elementType, atts);
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(element);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][3]);
                        break;
                    case 8:/// Settings -> Setting
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][0];
                        break;
                    case 9:/// Settings -> Settings Setting
                        AttributeToken nextset = _tokenStack[_tokenStack.Count - 1] as AttributeToken;
                        AttributeToken preset = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        nextset.Next = preset;
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(nextset);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][0]);
                        break;
                    case 10:/// Content -> USD Value Break
                        break;
                    case 11:/// Setting -> Sharp Att
                        _tokenStack.RemoveAt(_tokenStack.Count - _popCount[reducerule - 1]);
                        _stateStack.RemoveAt(_stateStack.Count - _popCount[reducerule - 1]);
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][1];
                        break;
                    case 12:/// Atts -> Att
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][4];
                        break;
                    case 13:/// Atts -> Atts Att
                        AttributeToken nextatt = _tokenStack[_tokenStack.Count - 1] as AttributeToken;
                        AttributeToken preatt = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        nextatt.Next = preatt;
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(nextatt);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][4]);
                        break;
                    case 14:/// Att -> Name Equals Value Break
                        AttributeValueType valueType = AttributeValueType.String;
                        switch (_tokenStack[_tokenStack.Count - 2].Type)
                        {
                            case TokenType.Number:
                                valueType = AttributeValueType.Number;
                                break;
                            case TokenType.Boolean:
                                valueType = AttributeValueType.Boolean;
                                break;
                        }
                        AttributeTypes attType = (AttributeTypes)System.Enum.Parse(typeof(AttributeTypes), _tokenStack[_tokenStack.Count - 4].Value);
                        AttributeToken att = new AttributeToken
                            (
                                _tokenStack[_tokenStack.Count - 2].Line,
                                attType,
                                _tokenStack[_tokenStack.Count - 2].Value,
                                valueType
                            );
                        _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        _tokenStack.Add(att);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][5]);
                        break;
                        //case 3://Settings -> Setting
                        //    _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][0];
                        //    break;
                        //case 4://Settings -> Settings Setting
                        //    AttributeToken nextset = _tokenStack[_tokenStack.Count - 1] as AttributeToken;
                        //    AttributeToken preset = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        //    nextset.Next = preset;
                        //    _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _tokenStack.Add(nextset);
                        //    _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][0]);
                        //    break;
                        //case 5://Setting -> Sharp Att
                        //    _tokenStack.RemoveAt(_tokenStack.Count - _popCount[reducerule - 1]);
                        //    _stateStack.RemoveAt(_stateStack.Count - _popCount[reducerule - 1]);
                        //    _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][1];
                        //    break;
                        //case 6://Elements -> Element
                        //    _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][2];
                        //    break;
                        //case 7://Elements -> Elements Element
                        //    ElementToken nextele = _tokenStack[_tokenStack.Count - 1] as ElementToken;
                        //    ElementToken preele = _tokenStack[_tokenStack.Count - 2] as ElementToken;
                        //    nextele.Next = preele;
                        //    _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _tokenStack.Add(nextele);
                        //    _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][2]);
                        //    break;
                        //case 8://Element -> EName Bracket Atts Bracket
                        //    var elementName = _tokenStack[_tokenStack.Count - 4].Text;
                        //    var elementType = (ElementTypes)System.Enum.Parse(typeof(ElementTypes), elementName);
                        //    var atts = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        //    ElementToken element = new ElementToken(atts.Line, elementType, atts);
                        //    _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _tokenStack.Add(element);
                        //    _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][3]);
                        //    break;
                        //case 9://Atts -> Att
                        //    _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][4];
                        //    break;
                        //case 10://Atts -> Atts Att
                        //    AttributeToken nextatt = _tokenStack[_tokenStack.Count - 1] as AttributeToken;
                        //    AttributeToken preatt = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        //    nextatt.Next = preatt;
                        //    _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _tokenStack.Add(nextatt);
                        //    _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][4]);
                        //    break;
                        //case 11://Att ->Name Equals Value Break
                        //    AttributeValueType valueType = AttributeValueType.String;
                        //    switch (_tokenStack[_tokenStack.Count - 2].Type)
                        //    {
                        //        case TokenType.Number:
                        //            valueType = AttributeValueType.Number;
                        //            break;
                        //        case TokenType.Boolean:
                        //            valueType = AttributeValueType.Boolean;
                        //            break;
                        //    }
                        //    AttributeTypes attType = (AttributeTypes)System.Enum.Parse(typeof(AttributeTypes), _tokenStack[_tokenStack.Count - 4].Text);
                        //    AttributeToken att = new AttributeToken
                        //        (
                        //            _tokenStack[_tokenStack.Count - 2].Line,
                        //            attType,
                        //            _tokenStack[_tokenStack.Count - 2].Text,
                        //            valueType
                        //        );
                        //    _tokenStack.RemoveRange(_tokenStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _stateStack.RemoveRange(_stateStack.Count - _popCount[reducerule - 1], _popCount[reducerule - 1]);
                        //    _tokenStack.Add(att);
                        //    _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][5]);
                        //    break;
                        //default:
                        //    break;
                }
            }

        }

    }

    internal class ParseException : System.Exception
    {
        internal ParseException() { }
        internal ParseException(string message) : base(message) { }
        internal ParseException(string message, System.Exception inner) : base(message, inner) { }
    }
}
