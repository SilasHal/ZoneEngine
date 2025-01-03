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
    class ScriptComponent
    {
        public IntPtr ScriptCreator;
    }

    [StructLayout(LayoutKind.Sequential)]
    class GameEntityDescriptor
    {
        public TransformComponent Transform = new TransformComponent();
        public ScriptComponent Script = new ScriptComponent();
    }
}

namespace ZoneEditor.DllWrappers
{
    static class EngineAPI
    {
        private const string _engineDll = "EngineDll.dll";

        [DllImport(_engineDll, CharSet = CharSet.Ansi)]
        public static extern int LoadGameCodeDll(string dllPath);

        [DllImport(_engineDll)]
        public static extern int UnloadGameCodeDll();

        [DllImport(_engineDll)]
        public static extern IntPtr GetScriptCreator(string name);

        [DllImport(_engineDll)]
        [return: MarshalAs(UnmanagedType.SafeArray)]
        public static extern string[] GetScriptNames();

        internal static class EntityAPI
        {
            [DllImport(_engineDll)]
            private static extern int CreateGameEntity(GameEntityDescriptor _descriptor);
            public static int CreateGameEntity(GameEntity entity)
            {
                GameEntityDescriptor _descriptor = new GameEntityDescriptor();

                //transform component
                {
                    var _component = entity.GetComponent<Transform>();
                    _descriptor.Transform.Position = _component.Position;
                    _descriptor.Transform.Rotation = _component.Rotation;
                    _descriptor.Transform.Scale = _component.Scale;
                }
                // script component
                {
                    //var _component = entity.GetComponent<Script>();
                    //_descriptor.Script.ScriptCreator = GetScriptCreator(_component.ScriptName);
                }

                return CreateGameEntity(_descriptor);
            }

            [DllImport(_engineDll)]
            private static extern void RemoveGameEntity(int id);
            public static void RemoveGameEntity(GameEntity entity)
            {
                RemoveGameEntity(entity.EntityId);
            }
        }
    }                                                 
}
