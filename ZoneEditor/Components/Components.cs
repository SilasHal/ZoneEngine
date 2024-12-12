using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace ZoneEditor.Components
{
    [DataContract]
    public class Components : ViewModelBase
    {
        [DataMember]
        public GameEntity Owner {  get; private set; }

        public Components(GameEntity owner)
        {
            Debug.Assert(owner != null);
            Owner = owner;
        }
    }
}
