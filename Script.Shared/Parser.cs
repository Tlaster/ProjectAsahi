using System.Collections.Generic;
using ScriptReader.Model;

namespace ScriptReader
{
    internal class Parser
    {
        private readonly string[][] _action = new string[][]
        {
            new[] {"S1",null,null,null,null,null,null,null },
            new[] {null,null,"S4",null,null,null,null,null },
            new[] {null,"ACC","S4",null,null,null,null,null },
            new[] {null,"R2","R2",null,null,null,null,null },
            new[] {null,null,null,null,null,null,null,"S7" },
            new string[] {null,null,null,null,null,null,null,null },
            new[] {null,"R3","R3",null,null,null,null,null },
            new[] {null,null,null,"S10",null,null,null,null },
            new[] {null,null,null,"S10",null,null,null,"S11" },
            new[] {null,null,null,"R5",null,null,null,"R5" },
            new[] {null,null,null,null,"S13",null,null,null },
            new[] {null,"R4","R4",null,null,null,null,null },
            new[] {null,null,null,"R6",null,null,null,"R6" },
            new[] {null,null,null,null,null,"S14",null,null },
            new[] {null,null,null,null,null,null,"S15",null },
            new[] {null,null,null,"R7",null,null,null,"R7" },
        };

        private readonly int[][] _goto = new int[][]
        {
            new[] {-1,-1,-1,-1 },
            new[] {2,3,-1,-1 },
            new[] {-1,6,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,8,9 },
            new[] {-1,-1,-1,12 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
            new[] {-1,-1,-1,-1 },
        };

        private int[] _popCount = new[] { 2, 1, 2, 4, 1, 2, 4 };

        internal bool IsAccepted { get; set; }
        internal bool IsLoopForReduce { get; set; }

        private List<Token> _tokenStack;
        private List<int> _stateStack;
        private BlockToken _blockToken;
        private Block _block;

        internal Parser()
        {
            _tokenStack = new List<Token>();
            _stateStack = new List<int>();
            _tokenStack.Add(new Token(0, null, TokenType.END));
            _stateStack.Add(0);
            IsLoopForReduce = false;
        }

        internal BlockToken BlockToken => _blockToken;
        internal Block Block => _block;

        internal void Reset()
        {
            _tokenStack = new List<Token>();
            _stateStack = new List<int>();
            _tokenStack.Add(new Token(0, null, TokenType.END));
            _stateStack.Add(0);
            _blockToken = null;
            IsLoopForReduce = false;
            IsAccepted = false;
        }

        internal void Push(Token token)
        {
            int wordtype = 0;
            switch (token.Type)
            {
                case TokenType.Number:
                case TokenType.Boolean:
                    wordtype = 5;
                    break;
                default:
                    wordtype = (int)token.Type;
                    break;
            }

            var action = _action[_stateStack[_stateStack.Count - 1]][wordtype];
            if (action == null)
            {
                IsLoopForReduce = false;
                throw new ParseException($"Grammar error at line {token.Line}");
            }
            else if (action == "ACC")
            {
                BlockTypes blocktype = (BlockTypes)System.Enum.Parse(typeof(BlockTypes), _tokenStack[1].Text);
                _blockToken = new BlockToken(blocktype, _tokenStack[2] as ElementToken);
                _block = new Block(_blockToken);
                IsAccepted = true;
                IsLoopForReduce = false;
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
                IsLoopForReduce = false;
            }
            else
            {
                IsLoopForReduce = true;
                var reducestring = "";
                for (int i = 1; i < action.Length; i++)
                {
                    reducestring += action[i];
                }
                var reducerule = int.Parse(reducestring);
                switch (reducerule)
                {
                    case 7://Redece Attribute
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
                        AttributeTypes attType = (AttributeTypes)System.Enum.Parse(typeof(AttributeTypes), _tokenStack[_tokenStack.Count - 4].Text);
                        AttributeToken att = new AttributeToken
                            (
                                _tokenStack[_tokenStack.Count - 2].Line,
                                attType,
                                _tokenStack[_tokenStack.Count - 2].Text,
                                valueType
                            );
                        _tokenStack.RemoveRange(_tokenStack.Count - 4, 4);
                        _stateStack.RemoveRange(_stateStack.Count - 4, 4);
                        _tokenStack.Add(att);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][3]);
                        break;
                    case 6:
                        AttributeToken nextatt = _tokenStack[_tokenStack.Count - 1] as AttributeToken;
                        AttributeToken preatt = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        nextatt.Next = preatt;
                        _tokenStack.RemoveRange(_tokenStack.Count - 2, 2);
                        _stateStack.RemoveRange(_stateStack.Count - 2, 2);
                        _tokenStack.Add(nextatt);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][2]);
                        break;
                    case 5:
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack[_stateStack.Count - 2]][2];
                        break;
                    case 4://Redece Element
                        var elementName = _tokenStack[_tokenStack.Count - 4].Text;
                        var elementType = (ElementTypes)System.Enum.Parse(typeof(ElementTypes), elementName);
                        var atts = _tokenStack[_tokenStack.Count - 2] as AttributeToken;
                        ElementToken element = new ElementToken(atts.Line, elementType, atts);
                        _tokenStack.RemoveRange(_tokenStack.Count - 4, 4);
                        _stateStack.RemoveRange(_stateStack.Count - 4, 4);
                        _tokenStack.Add(element);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][1]);
                        break;
                    case 3:
                        ElementToken nextele = _tokenStack[_tokenStack.Count - 1] as ElementToken;
                        ElementToken preele = _tokenStack[_tokenStack.Count - 2] as ElementToken;
                        nextele.Next = preele;
                        _tokenStack.RemoveRange(_tokenStack.Count - 2, 2);
                        _stateStack.RemoveRange(_stateStack.Count - 2, 2);
                        _tokenStack.Add(nextele);
                        _stateStack.Add(_goto[_stateStack[_stateStack.Count - 1]][0]);
                        break;
                    case 2:
                        _stateStack[_stateStack.Count - 1] = _goto[_stateStack.Count - 2][0];
                        break;
                    default:
                        break;
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
