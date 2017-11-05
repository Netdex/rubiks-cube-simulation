#pragma once

class shader
{
public:

	// Implement Custom Constructor and Destructor
	shader() { mProgram = glCreateProgram(); }
	~shader() { glDeleteProgram(mProgram); }

	// Public Member Functions
	shader&		activate();
	shader&		attach(std::string const & filename);
	GLuint		create(std::string const & filename);
	GLuint		get() const { return mProgram; }
	shader&		link();

	// Wrap Calls to glUniform
	static void bind(unsigned int location, float value);
	void		bind(unsigned int location, glm::mat4 const & matrix) const;
	void		bind(unsigned int location, glm::vec3 const & vec3) const;
	template<typename T> shader & bind(std::string const & name, T&& value)
	{
		int location = glGetUniformLocation(mProgram, name.c_str());
		if (location == -1) fprintf(stderr, "Missing Uniform: %s\n", name.c_str());
		else bind(location, std::forward<T>(value));
		return *this;
	}

private:

	// Disable Copying and Assignment
	shader(shader const &) = delete;
	shader & operator=(shader const &) = delete;

	// Private Member Variables
	GLuint mProgram;
	GLint  mStatus;
	GLint  mLength;

};
