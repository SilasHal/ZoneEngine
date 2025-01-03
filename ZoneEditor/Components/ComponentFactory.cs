using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ZoneEditor.Components;

namespace ZoneEditor.Components
{
    enum ComponentType
    {
        Transform,
        Script,
    }

    static class ComponentFactory
    {
        private static readonly Func<GameEntity, object, Component>[] _function =
            new Func<GameEntity, object, Component>[]
            {
                (entity, data)=> new Transform(entity),
                (entity, data)=>new Script(entity){Name = (string)data },
            };

        public static Func<GameEntity, object, Component> GetComponentFactory(ComponentType componentType)
        {
            Debug.Assert((int)componentType < _function.Length);
            return _function[(int)componentType];
        }
    }
}
