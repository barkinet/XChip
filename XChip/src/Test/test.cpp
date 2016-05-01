#include <csignal>
#include <iostream>

#include <XChip/Core/Emulator.h> 
#include <XChip/Media/SDLMedia.h>
#include <XChip/Utility/Memory.h>



static xchip::Emulator g_emulator;


int main(int argc, char **argv)
{
	using xchip::Emulator;
	using xchip::SdlRender;
	using xchip::SdlInput;
	using xchip::SdlSound;
	using xchip::UniqueRender;
	using xchip::UniqueInput;
	using xchip::UniqueSound;
	using xchip::utility::make_unique;




	if (argc < 2) {
		std::cout << "No game to load..." << std::endl;
		return EXIT_SUCCESS;
	}




	UniqueRender render;
	UniqueInput input;
	UniqueSound sound;


	try {
		render = make_unique<SdlRender>();
		input = make_unique<SdlInput>();
		sound = make_unique<SdlSound>();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	
	if (!g_emulator.Initialize(std::move(render), std::move(input), std::move(sound)))
		return EXIT_FAILURE;

	if (!g_emulator.LoadRom(argv[1]))
		return EXIT_FAILURE;



	if(signal(SIGINT, [](int signum)
	{
		std::cout << "Received signal: " << signum << std::endl;
		std::cout << "Closing Application!" << std::endl;
		g_emulator.SetExitFlag(true);

	}) == SIG_ERR )
	{
		std::cout << "Could not install signal handler!" << std::endl;
		return EXIT_FAILURE;
	}


	while (!g_emulator.GetExitFlag())
	{
		g_emulator.UpdateSystems(); 
		g_emulator.HaltForNextFlag();		
		if (g_emulator.GetInstrFlag()) 			
			g_emulator.ExecuteInstr();
		if (g_emulator.GetDrawFlag())
			g_emulator.Draw();

	}





	return EXIT_SUCCESS;


}














