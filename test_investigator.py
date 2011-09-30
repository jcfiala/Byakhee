import investigator
import skill

class mock(object):
    def __init__(self, values):
        self.values = values
    def __getattr__(self, name):
        if name in self.values:
            return self.values[name]

class TestInvestigatorSkillCase():
    def setup_method(self, method):
        self.skill = mock({'name' : 'Dodge', 'base' : '^DEX2', 'EraMask' : skill.ERA_ISALL, 'type' : skill.SKILL})
        #self.skill.name = "Dodge"
        #self.skill.base = "^DEX2"
        #self.skill.EraMask = skill.ERA_ISALL
        #self.skill.type = skill.SKILL
        self.inv_skill = investigator.InvestigatorSkill(self.skill, 50, 24)
    
    def test_mock_skill(self):
        assert self.skill.name == 'Dodge'
        assert self.skill.base == '^DEX2'
        assert self.skill.EraMask == skill.ERA_ISALL
        assert self.skill.type == skill.SKILL
    
    def test_create_investigator_skill(self):
        assert self.inv_skill.skill == self.skill
        assert self.inv_skill.current == 50
        assert self.inv_skill.minimum == 24
        assert self.inv_skill.occupational == False
        assert self.inv_skill.checked == False
        