using AurelienRibon.Ui.SyntaxHighlightBox;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

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
            shbox.CurrentHighlighter = HighlighterManager.Instance.Highlighters["Syntax"];
        }

        //private void richTextBox_TextChanged(object sender, TextChangedEventArgs e)
        //{

        //    TextRange range = new TextRange(richTextBox.Document.ContentStart, richTextBox.Document.ContentEnd);
        //    var start = richTextBox.Document.ContentStart;
        //    while (start != null && start.CompareTo(richTextBox.Document.ContentEnd) < 0)
        //    {
        //        if (start.GetPointerContext(LogicalDirection.Forward) == TextPointerContext.Text)
        //        {
        //            var match = _regex.Match(start.GetTextInRun(LogicalDirection.Forward));
        //            if (match.Success)
        //            {
        //                var textrange = new TextRange(start.GetPositionAtOffset(match.Index, LogicalDirection.Forward), start.GetPositionAtOffset(match.Index + match.Length, LogicalDirection.Backward));
        //                textrange.ApplyPropertyValue(TextElement.ForegroundProperty, new SolidColorBrush(Colors.Blue));
        //                textrange.ApplyPropertyValue(TextElement.FontWeightProperty, FontWeights.Bold);
        //                start = textrange.End; // I'm not sure if this is correct or skips ahead too far, try it out!!!
        //            }
        //        }
        //        start = start.GetNextContextPosition(LogicalDirection.Forward);
        //    }
        //}
    }
}
