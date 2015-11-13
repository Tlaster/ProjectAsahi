using ICSharpCode.AvalonEdit.CodeCompletion;
using ICSharpCode.AvalonEdit.Folding;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using ScriptWriter.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Xml;

namespace ScriptWriter
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow
    {
        private readonly FoldingManager _foldingManager;
        private readonly BraceFoldingStrategy _foldingStrategy;
        private CompletionWindow _completionWindow;
        private readonly StringBuilder _inputText;
        private IList<ICompletionData> _completeData { get; }

        private void OnComplete(CompletionData obj)
        {
            var index = textEditor.SelectionStart - _inputText.Length;
            textEditor.Text = textEditor.Text.Remove(index, _inputText.Length);
            textEditor.Text = textEditor.Text.Insert(index, obj.Text);
            textEditor.SelectionStart = index + obj.Text.Length;
            _inputText.Clear();
        }

        public MainWindow()
        {
            InitializeComponent();
            using (var reader = XmlReader.Create("Syntax.xshd"))
            {
                textEditor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
            }
            _completeData = new List<ICompletionData>()
            {
                new CompletionData(OnComplete,"#define"),
                new CompletionData(OnComplete,"#BGM"),
                new CompletionData(OnComplete,"#Background"),
                new CompletionData(OnComplete,"#Face"),
                new CompletionData(OnComplete,"#Content"),
                new CompletionData(OnComplete,"BGM"),
                new CompletionData(OnComplete,"Background"),
                new CompletionData(OnComplete,"Face"),
                new CompletionData(OnComplete,"Chara"),
                new CompletionData(OnComplete,"Content"),
                new CompletionData(OnComplete,"Path"),
                new CompletionData(OnComplete,"Position_X"),
                new CompletionData(OnComplete,"Position_Y"),
                new CompletionData(OnComplete,"Name"),
                new CompletionData(OnComplete,"Value"),
                new CompletionData(OnComplete,"Method"),
                new CompletionData(OnComplete,"Title"),
                new CompletionData(OnComplete,"Time"),
                new CompletionData(OnComplete,"Voice"),
                new CompletionData(OnComplete,"Deep"),
                new CompletionData(OnComplete,"NextFile"),
                new CompletionData(OnComplete,"MultipleLanguage"),
                new CompletionData(OnComplete,"TextLayoutBackground"),
                new CompletionData(OnComplete,"ImageWidth"),
                new CompletionData(OnComplete,"ImageHeight"),
                new CompletionData(OnComplete,"FontSize"),
                new CompletionData(OnComplete,"DefaultLanguage"),
                new CompletionData(OnComplete,"TAB"),
                new CompletionData(OnComplete,"SELECTION"),
            };

            _inputText = new StringBuilder();
            textEditor.TextArea.SelectionChanged += TextArea_SelectionChanged;
            textEditor.TextArea.PreviewKeyDown += TextArea_KeyDown;
            textEditor.TextArea.PreviewTextInput += TextArea_PreviewTextInput;
            _foldingManager = FoldingManager.Install(textEditor.TextArea);
            _foldingStrategy = new BraceFoldingStrategy();
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
            textEditor.TextChanged += TextEditor_TextChanged;
        }

        private void TextArea_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (e.Text.Length != 1) return;
            switch (e.Text)
            {
                case "{":
                    SetText("{ }", 2);
                    e.Handled = true;
                    break;

                case "=":
                    SetText("=\"\"", 2);
                    e.Handled = true;
                    break;

                case "\"":
                    if (textEditor.SelectionStart != textEditor.Text.Length && textEditor.Text[textEditor.SelectionStart] == '\"')
                        textEditor.SelectionStart++;
                    else
                        SetText("\"\"", 1);
                    e.Handled = true;
                    break;

                case "}":
                    if (textEditor.SelectionStart != textEditor.Text.Length && textEditor.Text[textEditor.SelectionStart] == '}')
                    {
                        textEditor.SelectionStart++;
                        e.Handled = true;
                    }
                    break;

                default:
                    {
                        if (e.Text != " " && e.Text != ";")
                        {
                            _inputText.Append(e.Text);
                        }
                        else
                        {
                            _inputText.Clear();
                        }
                        if (_inputText.Length > 0)
                        {
                            _completionWindow = new CompletionWindow(textEditor.TextArea);
                            foreach (var item in _completeData.Select(item => item.Text.StartsWith(_inputText.ToString(), StringComparison.CurrentCultureIgnoreCase) ? item : null))
                            {
                                if (item != null)
                                {
                                    _completionWindow.CompletionList.CompletionData.Add(item);
                                }
                            }
                            _completionWindow.Closed += delegate { _completionWindow = null; };
                            if (_completionWindow.CompletionList.CompletionData.Count > 0)
                            {
                                _completionWindow.Show();
                            }
                        }
                    }
                    break;
            }
        }

        private void TextArea_SelectionChanged(object sender, EventArgs e)//TODO:it doesn't work
        {
            _inputText.Clear();
        }

        private void TextArea_KeyDown(object sender, KeyEventArgs e)
        {
            if (((int)e.Key < 44 || (int)e.Key > 69) && e.Key != Key.Back)
            {
                _inputText.Clear();
            }
            else if (e.Key == Key.Back && _inputText.Length > 0)
            {
                _inputText.Remove(_inputText.Length - 1, 1);
            }
            switch (e.Key)
            {
                case Key.Return:
                    {
                        if (FindSyntaxPair('{', '}'))
                        {
                            BraceReturnHandler();
                            e.Handled = true;
                        }
                    }
                    break;

                case Key.Back:
                    {
                        if (textEditor.SelectionStart == textEditor.Text.Length || textEditor.SelectionStart == 0)
                            return;
                        if (FindSyntaxPair('{', '}', false) || FindSyntaxPair('\"', '\"', false))
                        {
                            var index = textEditor.SelectionStart - 1;
                            textEditor.Text = textEditor.Text.Remove(index, 2);
                            textEditor.SelectionStart = index;
                            e.Handled = true;
                        }
                    }
                    break;

                default:
                    break;
            }
        }

        private void BraceReturnHandler()
        {
            var index = textEditor.SelectionStart;
            int count = 1;
            int brpos = 0;
            for (int i = index; i > 0; i--)
            {
                if (textEditor.Text[i] == '\n')
                {
                    brpos = i;
                    break;
                }
            }
            for (int i = brpos + 1; i < index; i++)
            {
                var a = textEditor.Text[i];
                if (textEditor.Text[i] == '\t')
                    count++;
                else
                    break;
            }
            string tab = "";
            for (int i = 0; i < count; i++)
                tab += "\t";
            string insertstr = $"\n{tab}\n{tab.Remove(tab.Length - 1)}";
            textEditor.Text = textEditor.Text.Insert(index, insertstr);
            textEditor.SelectionStart = index + insertstr.Length - tab.Length;
        }

        private bool FindSyntaxPair(char firstSyntex, char secondSyntex, bool allowSpace = true)
        {
            var index = textEditor.SelectionStart;
            if (!allowSpace)
                return textEditor.Text[index - 1] == firstSyntex && textEditor.Text[index] == secondSyntex;
            bool isforward = false, isbackward = false;
            for (int i = index - 1; i >= 0; i--)
            {
                if (textEditor.Text[i] == firstSyntex)
                {
                    isforward = true;
                    break;
                }
                else if (textEditor.Text[i] != ' ' && textEditor.Text[i] != '\t')
                    break;
            }
            for (int i = index; i < textEditor.Text.Length; i++)
            {
                if (textEditor.Text[i] == secondSyntex)
                {
                    isbackward = true;
                    break;
                }
                else if (textEditor.Text[i] != ' ' && textEditor.Text[i] != '\t')
                    break;
            }
            return isforward && isbackward;
        }

        private void SetText(string insertStr, int select)
        {
            var index = textEditor.SelectionStart;
            textEditor.Text = textEditor.Text.Length == index ? textEditor.Text + insertStr : textEditor.Text.Insert(index, insertStr);
            textEditor.SelectionStart = index + select;
        }

        private void TextEditor_TextChanged(object sender, EventArgs e)
        {
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
        }
    }
}