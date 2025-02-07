﻿using System.Windows;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using ZoneEditor.GameProject;
using System.Diagnostics;
using ZoneEditor.Utilities;
using System.Windows.Media.Animation;

namespace ZoneEditor.GameDev
{
    /// <summary>
    /// Interaction logic for NewScriptDialog.xaml
    /// </summary>
    public partial class NewScriptDialog : Window
    {
        private static readonly string _cppCode = @"#include ""{0}.h""

namespace {1} {{

REGISTER_SCRIPT({0});
void {0}::begin_play()
{{

}}

void {0}::update(float detalTime)
{{

}}

}} // namespace {1}";

        private static readonly string _hCode = @"#pragma once
namespace {1} {{

class {0} : public zone::script::entity_script
{{
public:
    constexpr explicit {0}(zone::game_entity::entity entity)
        : zone::script::entity_script{{entity}} {{}}

    void begin_play() override;
    void update(float detalTime) override;
private:
}};
}} // namespace {1};";

        private static readonly string _namespace = GetNamespaceFromProjectName();

        private static string GetNamespaceFromProjectName()
        {
            var projectName = Project.Current.Name;
            projectName = projectName.Replace(" ", "_");
            return projectName;
        }

        private bool Validate()
        {
            bool isValid = false;
            var name = scriptName.Text.Trim();
            var path = scriptPath.Text.Trim();
            string errorMsg = string.Empty;
            if (string.IsNullOrEmpty(name))
            {
                errorMsg = "Type in a script name";
            }
            else if (name.IndexOfAny(Path.GetInvalidFileNameChars()) != -1 || name.Any(x => char.IsWhiteSpace(x)))
            {
                errorMsg = "Invalid character(s) used in script name.";
            }
            else if (string.IsNullOrEmpty(path))
            {
                errorMsg = "Script path cannot be empty";
            }
            else if (path.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                errorMsg = "Invalid character(s) used in script path.";
            }
            else if (!Path.GetFullPath(Path.Combine(Project.Current.Path, path)).Contains(Path.Combine(Project.Current.Path, @"GameCode\")))
            {
                errorMsg = "Script path must be within the GameCode folder";
            }
            else if (File.Exists(Path.GetFullPath(Path.Combine(Path.Combine(Project.Current.Path, path), $"{name}.cpp"))) ||
                     File.Exists(Path.GetFullPath(Path.Combine(Path.Combine(Project.Current.Path, path), $"{name}.h"))))
            {
                errorMsg = $"script {name} already exists in this folder.";
            }
            else
            {
                isValid = true;
            }

            if (!isValid)
            {
                messageTextBlock.Foreground = FindResource("Editor.RedBrush") as Brush;
            }
            else
            {
                messageTextBlock.Foreground = FindResource("Editor.FontBrush") as Brush;
            }
            messageTextBlock.Text = errorMsg;
            return isValid;
        }

        private void OnScriptName_TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (!Validate()) return;
            var name = scriptName.Text.Trim();
            messageTextBlock.Text = $"{name}.h and {name}.cpp will be created in the {Project.Current.Name}.";
        }

        private void OnScriptPath_TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Validate();
        }

        private async void OnCreate_Button_Click(object sender, RoutedEventArgs e)
        {
            if (!Validate()) return;
            IsEnabled = false;
            busyAnimation.Opacity = 0;
            busyAnimation.Visibility = Visibility.Visible;
            DoubleAnimation fadeIn = new DoubleAnimation(0, 1, new Duration(TimeSpan.FromMilliseconds(500)));
            busyAnimation.BeginAnimation(OpacityProperty, fadeIn);

            try
            {
                var name = scriptName.Text.Trim();
                var path = Path.GetFullPath(Path.Combine(Project.Current.Path, scriptPath.Text.Trim()));
                var solution = Project.Current.Solution;
                var projectName = Project.Current.Name;
                await Task.Run(() => CreateScript(name, path, solution, projectName));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to create script {scriptName.Text}");
            }
            finally
            {
                DoubleAnimation fadeOut = new DoubleAnimation(1, 0, new Duration(TimeSpan.FromMilliseconds(200)));
                fadeOut.Completed += (s, e) =>
                {
                    busyAnimation.Opacity = 0;
                    busyAnimation.Visibility = Visibility.Hidden;
                    Close();
                };
                busyAnimation.BeginAnimation(OpacityProperty, fadeOut);
            }
        }

        private void CreateScript(string name, string path, string solution, string projectName)
        {
            if (!Directory.Exists(path))
                Directory.CreateDirectory(path);

            var cppPath = Path.GetFullPath(Path.Combine(path, $"{name}.cpp"));
            var hPath = Path.GetFullPath(Path.Combine(path, $"{name}.h"));

            using (var sw = File.CreateText(cppPath))
            {
                sw.Write(string.Format(_cppCode, name, _namespace));
            }

            using (var sw = File.CreateText(hPath))
            {
                sw.Write(string.Format(_hCode, name, _namespace));
            }

            string[] files = new string[] { cppPath, hPath };

            for (int i = 0; i < 3; ++i)
            {
                if (!VisualStudio.AddFilesToSolution(solution, projectName, files))
                    System.Threading.Thread.Sleep(1000);

                else break;
            }
        }

        public NewScriptDialog()
        {
            InitializeComponent();
            Owner = Application.Current.MainWindow;
            scriptPath.Text = @"GameCode\";
        }
    }
}