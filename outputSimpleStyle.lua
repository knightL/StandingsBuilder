function printStandings ( file, teams, time )

	n=#teams
	p=contest.countproblems()

	for i,team in ipairs(teams) do
		file:write( contest.teamplace(team) .. ". \""..team.."\" - " ..
		contest.teamproblemssolved(team).."/"..p.." "..contest.teampenalty(team).."\n")
	end
end
