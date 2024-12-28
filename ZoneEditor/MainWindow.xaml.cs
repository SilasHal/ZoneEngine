using System.ComponentModel;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ZoneEditor.GameProject;

namespace ZoneEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static string ZonePath { get; private set; } = @"E:\Station\github\ZoneEngine";

        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
            Closing += OnMainWindowClosing;
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnMainWindowLoaded;
            OpenProjectBrowserDialog();
        }
        private void OnMainWindowClosing(object? sender, CancelEventArgs e)
        {
            Closing -= OnMainWindowClosing;
            Project.Current?.Unload();
        }

        private void OpenProjectBrowserDialog()
        {
            var projectBrowser = new ProjectBrowserDialog();
            if (projectBrowser.ShowDialog() == false || projectBrowser.DataContext == null) 
            {
                Application.Current.Shutdown();
            }
            else
            {
                Project.Current?.Unload();
                DataContext = projectBrowser.DataContext;
            }
        }
    }
}