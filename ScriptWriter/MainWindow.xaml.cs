using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Folding;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using ScriptWriter.Common;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Resources;
using System.Text.RegularExpressions;
using System.Windows.Input;
using System.Xml;
using System.Linq;

namespace ScriptWriter
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary> 
    public partial class MainWindow
    {
        private FoldingManager _foldingManager;
        private BraceFoldingStrategy _foldingStrategy;

        public MainWindow()
        {
            InitializeComponent();
            using (XmlReader reader = XmlReader.Create("Syntax.xshd"))
            {
                textEditor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
            }
            textEditor.TextArea.PreviewKeyDown += TextArea_KeyDown;
            textEditor.TextArea.PreviewTextInput += TextArea_PreviewTextInput;
            _foldingManager = FoldingManager.Install(textEditor.TextArea);
            _foldingStrategy = new BraceFoldingStrategy();
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
            textEditor.TextChanged += TextEditor_TextChanged;
        }

        private void TextArea_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.Key)
            {
                case Key.Return:
                    {
                        if (FindSyntaxPair('{','}'))
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
            var str = textEditor.Text.Split('\n');
            int count = 1;
            int brpos = 0;
            for (int i = index; i > 0; i--)
            {
                var a = textEditor.Text[i];
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

        private bool FindSyntaxPair(char firstSyntex, char secondSyntex,bool allowSpace = true)
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
                else if(textEditor.Text[i] != ' ' && textEditor.Text[i] != '\t')
                    break;
            }
            for (int i = index; i < textEditor.Text.Length ; i++)
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

        private void TextArea_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (e.Text.Length == 1)
            {
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
                        break;
                }
            }
        }

        private void SetText(string insertStr, int select)
        {
            var index = textEditor.SelectionStart;
            textEditor.Text = textEditor.Text.Length == index ? textEditor.Text + insertStr : textEditor.Text.Insert(index, insertStr);
            textEditor.SelectionStart = index + select;
        }

        private void TextEditor_TextChanged(object sender, System.EventArgs e)
        {
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
        }
    }
}
