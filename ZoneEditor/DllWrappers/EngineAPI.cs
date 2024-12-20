using System;
using System.Collections.Generic;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using ZoneEditor.Components;
using ZoneEditor.EngineAPIStructs;

namespace ZoneEditor.EngineAPIStructs
{
    [StructLayout(LayoutKind.Sequential)]
    class TransformComponent
    {
        public Vector3 Position;
        public Vector3 Rotation;
        public Vector3 Scale = new Vector3(1, 1, 1);
    }

    [StructLayout(LayoutKind.Sequential)]
    class GameEntityDescriptor
    {
        public TransformComponent Transform = new TransformComponent();
    }
}

namespace ZoneEditor.DllWrappers
{
    static class EngineAPI
    {
        private const string _dllName = "EngineDll.dll";

        [DllImport(_dllName)]
        private static extern int CreateGameEntity(GameEntityDescriptor _descriptor);
        public static int CreateGameEntity(GameEntity entity)
        {
            GameEntityDescriptor _descriptor = new GameEntityDescriptor();
            //transform component
            {
                var _component = entity.GetComponent<Transform>();
                _descriptor.Transform.Position = _component.Positon;
                _descriptor.Transform.Rotation = _component.Rotation;
                _descriptor.Transform.Scale = _component.Scale;
            }
            return CreateGameEntity(_descriptor);
        }

        [DllImport(_dllName)]
        private static extern void RemoveGameEntity(int id);
        public static void RemoveGameEntity(GameEntity entity)
        {
            RemoveGameEntity(entity.EntityId);
        }

    }                                                 
}
