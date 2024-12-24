using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace ZoneEditor.Utilities.Controls
{
    [TemplatePart(Name = "PART_textBlock",Type =typeof(TextBlock))]
    [TemplatePart(Name = "PART_textBox", Type = typeof(TextBox))]
    class NumberBox : Control
    {
        public string Value
        {
            get => (string)GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }
        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(nameof(Value), typeof(string), typeof(NumberBox), 
                                                                                              new FrameworkPropertyMetadata(null,FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            if (GetTemplateChild("PART_textBlock") is TextBlock textBlock)
            {
                textBlock.MouseLeftButtonDown += OnTextBlock_Mouse_LBD;
                textBlock.MouseLeftButtonUp += OnTextBlock_Mouse_LBU;
                textBlock.MouseMove += OnTextBlock_Mouse_Move;
            }
        }

        private void OnTextBlock_Mouse_Move(object sender, MouseEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void OnTextBlock_Mouse_LBU(object sender, MouseButtonEventArgs e)
        {
            throw new NotImplementedException();
        }

        private void OnTextBlock_Mouse_LBD(object sender, MouseButtonEventArgs e)
        {
            throw new NotImplementedException();
        }

        static NumberBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(NumberBox),new FrameworkPropertyMetadata(typeof(NumberBox)));
        }
    }
}
