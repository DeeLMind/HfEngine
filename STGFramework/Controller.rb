#encoding :utf-8
module Controller
	def self.init
		@fps = $config[:logic].fps or 60
		@timer = FPSTimer.new(@fps)
		
		@keyboard = DX::Input::Keyboard.new($window)
	end
	
	def self.fps
		return @fps
	end
	def self.fps=(f)
		@fps = f
		restart
	end
	def self.wait_next_frame
		@timer.await
	end
	def self.restart
		@timer.restart(@fps)
	end
	def self.keyboard
		return @keyboard
	end
	
	def self.shutdown
		@keyboard.release
	end
end