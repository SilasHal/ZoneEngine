using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZoneEditor.GameProject
{
    public class Scene :ViewModelBase
    {
        private string _name = "MyProject";
        public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        public Project Project { get; private set; }
        public Scene(Project project, string name) 
        { 
            Debug.Assert(project != null);
            Project = project;
            Name = name;
        }
    }
}
