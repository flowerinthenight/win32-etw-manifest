#pragma once

namespace LibRTWrapperETW
{
    public ref class TraceCore sealed
	{
	private:
		TraceCore();
		~TraceCore();
		static TraceCore^ m_Instance;

	public:
		static property TraceCore^ Instance
		{
			TraceCore^ get()
			{
				if (m_Instance == nullptr)
				{
					m_Instance = ref new TraceCore();
				}

				return m_Instance;
			}
		}

		void Verbose(Platform::String^ mod, Platform::String^ file, Platform::String^ func, Platform::String^ m);
	};
}
