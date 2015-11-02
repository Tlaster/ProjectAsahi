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
using System.Xml;

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
            _foldingManager = FoldingManager.Install(textEditor.TextArea);
            _foldingStrategy = new BraceFoldingStrategy();
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
            textEditor.TextChanged += TextEditor_TextChanged;
        }

        private void TextEditor_TextChanged(object sender, System.EventArgs e)
        {
            _foldingStrategy.UpdateFoldings(_foldingManager, textEditor.Document);
        }
    }
}
