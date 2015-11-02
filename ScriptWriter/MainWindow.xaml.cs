using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using System.Collections;
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
        private Regex _regex = new Regex("(BGM|Chara|Background|Face|Content)|(Path|Position_X|Position_Y|Name|Value|Method|Title|Time|Voice|Deep|NextFile|MultipleLanguage|TextLayoutBackground|ImageWidth|ImageHeight|FontSize|DefaultLanguage)|(TAB|SELECTION)|(#define|#)|(true|false)");
        public MainWindow()
        {
            InitializeComponent();
            using (XmlReader reader = XmlReader.Create("Syntax.xml"))
            {
                textEditor.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
            }
        }
    }
}
